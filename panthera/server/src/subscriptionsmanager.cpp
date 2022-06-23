#include "subscriptionsmanager.h"

#include <boost/lexical_cast.hpp>


template<> void
SubscriptionsManager::Impl<DataMode::Mode_1>::exec(  )
{
    std::unique_lock<std::mutex> lock( _clientsMutex, std::defer_lock );
    DataSource<DataMode::Mode_1> dataSource( _marketData.records<MarketDataRecordsPtr>( ) );
    MarketDataRecord r;
    dataSource.findFirst( );
    while( dataSource.fetchNext( r, _terminate ) ) {
        lock.lock( );
        if ( _terminate ) {
            break;
        }
        auto iter = _clients.find( r.symbol );
        if ( iter == _clients.end( ) ) {
            lock.unlock( );
            continue;
        }
        auto & connections = iter->second;
        for( auto i = connections.begin( ); i != connections.end( ); ) {
            if ( ! (*i)->isActive( ) ) {
                i = connections.erase( i );
            }
            else {
                ++i;
            }
        }
        lock.unlock( );
        for( auto & connection : connections ) {
            connection->send( r );
        }
    }
}


template<> void
SubscriptionsManager::Impl<DataMode::Mode_2>::exec(  )
{
    MarketDataRecord r;
    std::unique_lock<std::mutex> lock( _clientsMutex, std::defer_lock );
    while( ! _terminate ) {
        lock.lock( );
        if ( _clients.empty( ) ) {
            _clientsCondition.wait( lock, [&] { return ! _clients.empty( ) || _terminate; } );
        }
        if ( _terminate || _clients.empty( ) ) {
            continue;
        }
        auto [connection, dataSource] = std::move( _clients.front( ) );
        _clients.pop( );
        lock.unlock( );

        dataSource.findFirst( );
        while( dataSource.fetchNext( r, _terminate ) ) {
            connection->send( r );
        }
    }
}


template<>
SubscriptionsManager::Impl<DataMode::Mode_1>::Impl( MarketData<DataMode::Mode_1> && md ) noexcept :
        _marketData( std::move( md ) )
{
    // TODO: Just hardcode one for now. Doesn't seem to make much of a difference. Needs benchmarking and checking the implementation
    std::thread feed_thread( run, this, std::string( "MarketFeedThread" ) );
    Util::setThreadAffinity( feed_thread.native_handle( ), 0 );
    _feedThreads.push_back( std::move( feed_thread ) );
}

template<>
SubscriptionsManager::Impl<DataMode::Mode_2>::Impl( MarketData<DataMode::Mode_2> && md ) noexcept :
        _marketData( std::move( md ) )
{
    auto count = std::thread::hardware_concurrency( );
    count = ! count ? 1 : count;
    for( auto i = 0; i < count; ++i ) {
        _feedThreads.push_back( std::thread( run, this, std::string( "MarketFeedThread" ) + boost::lexical_cast<std::string>( i )  ) );
    }
}


template<> void
SubscriptionsManager::Impl<DataMode::Mode_1>::subscribeTo( TcpConnection::Ptr connection )
{
    std::unique_lock<std::mutex> lock( _clientsMutex );
    _clients[ connection->symbol( ) ].push_back( connection );
}


template<> void
SubscriptionsManager::Impl<DataMode::Mode_2>::subscribeTo( TcpConnection::Ptr connection )
{
    {
        std::unique_lock<std::mutex> lock( _clientsMutex );
        _clients.push( std::make_pair(  connection,
                                        DataSource<DataMode::Mode_2>( _marketData.records<MarketDataRecordsPtr>( connection->symbol( ) ),
                                                                      connection->timestamp( )
                                                                      )
                                      )
                      );
    }
    _clientsCondition.notify_one( );
}


