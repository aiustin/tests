#ifndef SERVER_TCPCONNECTION_H
#define SERVER_TCPCONNECTION_H


#include <memory>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "marketdatarecord.h"

using boost::asio::ip::tcp;

class SubscriptionsManager;
class Subscription;

/**
 * The TcpConnection class provides the core I/O functionality for managing a subscription connection
 * Once a connection is created it will async_read the subscription message, validate it and
 * create a subscription.
 *
 */


class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    ~TcpConnection( );

    typedef std::shared_ptr<TcpConnection> Ptr;

    /// Factory method. Constructor is private.
    static Ptr create( tcp::socket::executor_type executor ) {
        return Ptr( new TcpConnection( executor ) );
    }

    /// Accessors for Connection's properties
    tcp::socket & socket( ) noexcept {
        return _socket;
    }
    const std::array<char, 10> & subscriptionMessage( ) const noexcept {
        return _subscriptionMessage;
    }
    const MarketSymbol & symbol( ) const noexcept {
        return _symbol;
    }
    const MarketTimestamp & timestamp( ) const noexcept {
        return _timestamp;
    }

    /// Connection is active once a valid subscription message was received and
    /// connection was binded to a subscription.
    /// It becomes not active if a send fails.
    bool isActive( ) const noexcept {
        return _active;
    }

    /// Not-blocking reading the subscribe message & subscribing
    void openAndSubscribe( const SubscriptionsManager & manager );
    /// Sends a MarketDataRecord through socket, if active
    void send( const MarketDataRecord & r );

private:
    TcpConnection( tcp::socket::executor_type executor ) noexcept;

    void handleRead( const boost::system::error_code & error, size_t bytes_transferred );

private:
    tcp::socket _socket;
    std::array<char, 10> _subscriptionMessage { };
    std::unique_ptr<Subscription> _subscription;
    MarketSymbol _symbol;
    MarketTimestamp _timestamp;
    bool _active = false;
};




#endif //SERVER_TCPCONNECTION_H
