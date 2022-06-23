#include "subscription.h"

void
Subscription::bind( TcpConnection::Ptr && connection ) noexcept
{
    if ( _manager.isNull( ) ) {
        return;
    }
    _manager.subscribeTo( std::move( connection ) );
    _manager = SubscriptionsManager( nullptr );
}