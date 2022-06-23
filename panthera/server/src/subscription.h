#ifndef SERVER_SUBSCRIPTION_H
#define SERVER_SUBSCRIPTION_H

#include "subscriptionsmanager.h"
#include "tcpconnection.h"

/**
 * The Subscription class is just a helper class to transparently allow TcpConnection not to
 *  keep a reference to SubscriptionManager once subscribed.
 *  There should be a better solution to this.
 */

class Subscription {
public:
    Subscription( const SubscriptionsManager & manager ) noexcept : _manager( manager ) { }
    void bind( TcpConnection::Ptr && connection ) noexcept;

private:
    SubscriptionsManager _manager;
};


#endif //SERVER_SUBSCRIPTION_H
