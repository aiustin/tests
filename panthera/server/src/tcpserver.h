#ifndef SERVER_TCPSERVER_H
#define SERVER_TCPSERVER_H

#include "tcpconnection.h"
#include "subscriptionsmanager.h"

/**
 * The TcpServer class provides the core I/O functionality for accepting & creating new
 * subscription connections
 *
 */


class TcpServer {
public:
    /// Constructor - requires a SubscriptionsManager to be passed along to
    ///                 newly created connections.
    TcpServer( const SubscriptionsManager & manager );

    /// Opens the port and blocks running the event loop
    void run( unsigned short port );
    /// Shuts down the server
    void shutdown( );

private:
    void accept( );
    void handleAccept( TcpConnection::Ptr connection, const boost::system::error_code & error );

private:
    boost::asio::io_context _context;
    tcp::acceptor _acceptor;
    SubscriptionsManager _manager;
};


#endif //SERVER_TCPSERVER_H
