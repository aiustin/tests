#include "tcpserver.h"


TcpServer::TcpServer( const SubscriptionsManager & manager ) :
    _acceptor( _context )
,   _manager( manager )
{
}

void
TcpServer::run( unsigned short port )
{
    if ( _acceptor.is_open( ) ) {
        return;
    }
    _acceptor = tcp::acceptor( _context, tcp::endpoint(tcp::v4(), port ) );
    accept( );
    _context.run( );
}

void
TcpServer::shutdown( )
{
    _context.stop( );
    _manager.shutdown( );
}


void
TcpServer::accept( )
{
    TcpConnection::Ptr new_connection = TcpConnection::create( _acceptor.get_executor( ) );

    _acceptor.async_accept(new_connection->socket(),
                           boost::bind( &TcpServer::handleAccept, this, new_connection,
                                        boost::asio::placeholders::error
                                        )
                           );
}


void
TcpServer::handleAccept( TcpConnection::Ptr connection, const boost::system::error_code & error )
{
    if ( ! error ) {
        connection->openAndSubscribe( _manager );
        accept( );
    }
    else {
        // TODO: Log error
    }
}

