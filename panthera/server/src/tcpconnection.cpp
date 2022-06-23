#include "tcpconnection.h"

#include "subscription.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>


//TODO: this should be in some client-server common header

// Network struct for sending market data
// using alignas to avoid pack()
struct alignas(16) TcpMarketDataRecord {
    uint8_t symbol[4];
    uint32_t timestampSecs;
    uint32_t timestampMicrosecs;
    uint16_t lastPriceInt;
    uint16_t lastPriceDec;
};
static_assert( sizeof( TcpMarketDataRecord ) == 16, "Wrong size for TcpMarketDataRecord" );


TcpConnection::TcpConnection( tcp::socket::executor_type executor ) noexcept :
    _socket( executor )
{
}

TcpConnection::~TcpConnection( )
{

}

void
TcpConnection::openAndSubscribe( const SubscriptionsManager & manager )
{
    _subscription.reset( new Subscription( manager ) );

    boost::asio::async_read( _socket, boost::asio::buffer(_subscriptionMessage ),
                             boost::bind( &TcpConnection::handleRead, shared_from_this( ),
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred
                                          )
                            );
}


void
TcpConnection::send( const MarketDataRecord & r )
{
    if ( ! _active ) {
        return;
    }

    //TODO: add some generic methods for serializing/deserializing a MarketDataRecord
    TcpMarketDataRecord tcp_r;
    std::copy( r.symbol.begin( ), r.symbol.end( ), tcp_r.symbol );
    auto us = r.timestamp.time_since_epoch( );
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>( us );
    us -= std::chrono::duration_cast<std::chrono::microseconds>( seconds );
    tcp_r.timestampSecs = htonl( seconds.count( ) );
    tcp_r.timestampMicrosecs = htonl( us.count( ) );
    tcp_r.lastPriceInt = htons( r.lastPriceInt );
    tcp_r.lastPriceDec = htons( r.lastPriceDec );

    try {
        boost::asio::write( _socket, boost::asio::buffer( &tcp_r, sizeof( tcp_r ) ) );
    }
    catch( boost::system::system_error & ) {
        _active = false;
        _socket.close( );
    }

}


void
TcpConnection::handleRead( const boost::system::error_code & error, size_t bytes_transferred )
{
    // TODO: all this parsing  and error checking is awful.
    assert( bytes_transferred == sizeof( _subscriptionMessage ) );
    if ( error.failed( ) ) {
        assert( false );
        return;
    }

    std::array<char, 11> temp {};
    std::copy( _subscriptionMessage.begin( ), _subscriptionMessage.end( ), temp.begin( ) );
    std::string msg( temp.data( ) );
    std::vector<std::string> tokens;
    boost::algorithm::split( tokens, msg, boost::algorithm::is_any_of( std::string { SymbolSeparator, TimestampSeparator } ) );

    assert( tokens.size( ) == 3 );
    assert( tokens[ 0 ].size( ) > 0 );
    assert( tokens[ 1 ].size( ) > 0 && tokens[ 1 ].size( ) <= 2 );
    assert( tokens[ 2 ].size( ) > 0 && tokens[ 2 ].size( ) <= 2 );
    if ( tokens.size( ) != 3  || ! tokens[ 0 ].size( ) ||
         ! tokens[ 1 ].size( ) || tokens[ 1 ].size( ) > 2 ||
         ! tokens[ 2 ].size( ) || tokens[ 2 ].size( ) > 2
    ) {
        return;
    }

    // TODO: make utility functions for all these conversions
    boost::to_upper( tokens[ 0 ] );
    _symbol.fill( 0 );
    std::copy( tokens[ 0 ].begin( ), tokens[ 0 ].end( ), _symbol.begin( ) );

    try {
        const auto hours = boost::lexical_cast<short>( tokens[ 1 ] );
        const auto minutes = boost::lexical_cast<short>( tokens[ 2 ] );
        assert( hours >= 0 && hours <= 23 );
        assert( minutes >= 0 && minutes <= 59 );
        _timestamp += std::chrono::hours( hours );
        _timestamp += std::chrono::minutes ( minutes );
    }
    catch ( const boost::bad_lexical_cast & ) {
        assert( false );
        return;
    }
    _active = true;

    // If everything alright add a new subscription
    _subscription->bind( shared_from_this( ) );
}
