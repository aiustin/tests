#ifndef SERVER_SUBSCRIPTIONSMANAGER_H
#define SERVER_SUBSCRIPTIONSMANAGER_H

#include "tcpconnection.h"
#include "marketdata.h"
#include "datasource.h"
#include "util.h"

#include <queue>
#include <unordered_map>

/**
 * The SubscriptionsManager class provides the interface for subscriptions
 *
 * In Mode_1:
 *  - connections are grouped by symbol
 *  - only one thread is used named MarketFeedThread which feches records one by one and sends them to
 *  subscribed clients
 *
 * In Mode_2:
 *  - multiple threads are used - as a thread pool
 *  - when a subscription comes one of the threads is woken up and will be serving it
 *  - TODO: improve the algorithm for matching subscriptions with threads
 *
 */

template<DataMode mode>
struct ClientsContainer {
	struct type {}; 
};
template<> struct ClientsContainer<DataMode::Mode_1> { using type = std::unordered_map<MarketSymbol, std::list<TcpConnection::Ptr>, boost::hash<MarketSymbol>>; };
template<> struct ClientsContainer<DataMode::Mode_2> { using type = std::queue<std::pair<TcpConnection::Ptr, DataSource<DataMode::Mode_2>>>; };


class SubscriptionsManager {
public:
    template<DataMode mode>
    static SubscriptionsManager initWith( MarketData<mode> && md ) noexcept;

    bool isNull( ) const noexcept { return ! _impl.get( ); }

    void subscribeTo( TcpConnection::Ptr connection ) { _impl->subscribeTo( connection ); }
    void shutdown( ) { _impl->shutdown( ); }


private:
    struct Private {
        virtual ~Private( ) noexcept { }
        virtual void subscribeTo( TcpConnection::Ptr connection ) =0;
        virtual void shutdown( ) =0;
    };

    template<DataMode mode>
    class Impl : public Private {
    public:
        Impl( MarketData<mode> && md ) noexcept : _marketData( std::move( md ) ) { }
        void subscribeTo( TcpConnection::Ptr connection ) override;
        void shutdown( ) override;

    private:
        static void run( Impl * impl, std::string threadName );
        void exec( );
    private:

    private:
        MarketData<mode> _marketData;
        std::vector<std::thread> _feedThreads;
        std::mutex _clientsMutex;
        std::condition_variable _clientsCondition;
        std::atomic_bool _terminate { false };
        typename ClientsContainer<mode>::type _clients;
    };

private:
    SubscriptionsManager( std::shared_ptr<Private> p ) noexcept : _impl( p ) { }
private:
    std::shared_ptr<Private> _impl;

    friend class Subscription;
};



template<DataMode mode> void
SubscriptionsManager::Impl<mode>::shutdown( )
{
    _terminate.store( true );
    _clientsCondition.notify_all( );
    for( auto i = 0; i < _feedThreads.size( ); ++i ) {
        _feedThreads[ i ].join( );
    }
}

/*static */ template<DataMode mode> void
SubscriptionsManager::Impl<mode>::run( SubscriptionsManager::Impl<mode> * impl, std::string threadName )
{
    Util::setThreadName( threadName.c_str( ) );
    impl->exec( );
}


template<DataMode mode> /*static */ SubscriptionsManager
SubscriptionsManager::initWith( MarketData<mode> && md ) noexcept
{
    return SubscriptionsManager( std::shared_ptr<Private>( new Impl<mode>( std::move( md ) ) ) );
}


#endif //SERVER_SUBSCRIPTIONSMANAGER_H
