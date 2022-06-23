#include <iostream>

#include "tcpserver.h"
#include "marketdata.h"

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

// Not a big fan of 'using' but these boost prefixes are quite long
using boost::asio::ip::tcp;

TcpServer * RunningServer = nullptr;

static const char * MarketData_FileName = "market_data.csv";

void sigHandler( int s );


int
main()
{

    std::ifstream f( MarketData_FileName );
    if ( ! f.good( ) ) {
        std::cout << "Data file " << MarketData_FileName << " not found.";
        return 1;
    }

    // Installing shutdown handlers
    struct sigaction sigint_handler;
    sigint_handler.sa_handler = sigHandler;
    sigemptyset( & sigint_handler.sa_mask );
    sigint_handler.sa_flags = 0;
    sigaction( SIGINT, & sigint_handler, 0 );
    sigaction( SIGTERM, & sigint_handler, 0 );
    sigaction( SIGQUIT, & sigint_handler, 0 );


    // Reading configuration info Mode/Port
    auto mode_str =  std::getenv( "MD_SERVER_MODE" );
    auto port_str =  std::getenv( "MD_SERVER_PORT" );

    unsigned short port_num = 3333;
    if ( port_str ) {
        port_num = boost::lexical_cast<unsigned short>( port_str );
    }

    unsigned short mode_num = 1;
    if ( mode_str ) {
        mode_num = boost::lexical_cast<unsigned short>( mode_str );
    }

    // Running the server
    DataMode mode = static_cast<DataMode>( mode_num );

    try {
        switch( mode ) {
            case DataMode::Mode_1:
                RunningServer = new TcpServer( SubscriptionsManager::initWith( MarketData<DataMode::Mode_1>::fromCSV( MarketData_FileName ) ) );
                break;
            case DataMode::Mode_2:
                RunningServer = new TcpServer( SubscriptionsManager::initWith( MarketData<DataMode::Mode_2>::fromCSV( MarketData_FileName ) ) );
                break;
            default:
                assert( false );
                break;
        }
        if ( RunningServer ) {
            std::cout << "Running in mode " << mode_num << " on port " << port_num << std::endl;
            RunningServer->run( port_num );
            RunningServer->shutdown( );
        }
    }
    catch( boost::system::system_error & e ) {
        std::cerr << e.what( ) << std::endl;
    }
    catch ( std::exception & e ) {
        std::cerr << e.what( ) << std::endl;
    }
    delete RunningServer;

    return 0;
}


void
sigHandler( int s )
{
    if ( RunningServer ) {
        std::cout << "Shutting down..." << std::endl;
        RunningServer->shutdown( );
        delete RunningServer;
    }
    exit( 1 );
}

