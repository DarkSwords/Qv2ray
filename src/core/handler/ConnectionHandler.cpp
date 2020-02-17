#include "ConnectionHandler.hpp"
namespace Qv2ray::core::handlers
{
    template<typename IDType>
    QList<IDType> StringsToIdList(const QList<QString> &strings)
    {
        QList<IDType> list;

        for (auto str : strings) {
            list << IDType(str);
        }

        return list;
    }

    QvConnectionHandler::QvConnectionHandler()
    {
        DEBUG(MODULE_CORE_HANDLER, "ConnectionHandler Constructor.")

        // Do we need to check how many of them are loaded?
        for (auto i = 0; i < GlobalConfig.connections.count(); i++) {
            connections[ConnectionId(GlobalConfig.connections.keys()[i])] = GlobalConfig.connections.values()[i];
        }

        for (auto i = 0; i < GlobalConfig.subscriptions.count(); i++) {
            subscriptions[SubscriptionId(GlobalConfig.subscriptions.keys()[i])] = GlobalConfig.subscriptions.values()[i];
        }

        for (auto i = 0; i < GlobalConfig.groups.count(); i++) {
            groups[GroupId(GlobalConfig.groups.keys()[i])] = GlobalConfig.groups.values()[i];
        }

        kernelInstance = new V2rayKernelInstance();
        saveTimerId = startTimer(60000);
    }

    const QList<GroupId> QvConnectionHandler::Groups() const
    {
        return groups.keys();
    }
    const QList<SubscriptionId> QvConnectionHandler::Subscriptions() const
    {
        return subscriptions.keys();
    }
    const QList<ConnectionId> QvConnectionHandler::Connections() const
    {
        return connections.keys();
    }
    const QList<ConnectionId> QvConnectionHandler::Connections(const GroupId &groupId) const
    {
        return StringsToIdList<ConnectionId>(groups[groupId].connections);
    }
    const QList<ConnectionId> QvConnectionHandler::Connections(const SubscriptionId &subscriptionId) const
    {
        return StringsToIdList<ConnectionId>(subscriptions[subscriptionId].connections);
    }

    const QvConnectionObject &QvConnectionHandler::GetConnection(const ConnectionId &id)
    {
        if (!connections.contains(id)) {
            LOG(MODULE_CORE_HANDLER, "Cannot find id: " + id.toString());
        }

        return connections[id];
    }

    const QvGroupObject &QvConnectionHandler::GetGroup(const GroupId &id)
    {
        return groups[id];
    }

    const QvSubscriptionObject &QvConnectionHandler::GetSubscription(const SubscriptionId &id)
    {
        return subscriptions[id];
    }

    QvConnectionHandler::~QvConnectionHandler()
    {
        if (kernelInstance->KernelStarted) {
            kernelInstance->StopConnection();
            LOG(MODULE_CORE_HANDLER, "Stopped connection from destructor.")
        }

        delete kernelInstance;
    }
}
