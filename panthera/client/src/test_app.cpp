
//
// test_app.cpp
//

#include "test_app.h"

#include <iostream>
#include <string>
#include <array>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

using boost::asio::ip::tcp;


// ==========================================================================================================
namespace test
{
    // ======================================================================================================
    TestApp::TestApp()
    {

    }

    // ======================================================================================================
    TestApp::~TestApp()
    {

    }

    // ======================================================================================================
    struct alignas(16) TcpMarketDataRecord {
        uint8_t symbol[4];
        uint32_t timestampSecs;
        uint32_t timestampMicrosecs;
        uint16_t lastPriceInt;
        uint16_t lastPriceDec;
    };

    void TestApp::run( int argc, char* argv[] )
    {
        try
        {
            //TODO:  Use boost::program_options
            if (argc < 2)
            {
                std::cerr << "Usage: client <symbol> [<ip>] [<port>]" << std::endl;
                std::cerr << "\t e.g.: client AMD@09:10" << std::endl;
                std::cerr << "\t e.g.: client INTC@10:00 192.168.10.1 1333" << std::endl;
                return;
            }

            std::array<char, 10> subscription {};
            std::copy( argv[ 1 ], argv[ 1 ] + strlen(argv[ 1 ]), subscription.begin( ) );
            std::string host = "127.0.0.1";
            unsigned short port = 3333;
            if ( argc > 2 ) {
                host = argv[2];
            }
            if ( argc > 3 ) {
                port = boost::lexical_cast<unsigned short>( argv[3] );
            }

            //std::cout << "Connecting to " << host << " on port " << port << std::endl;

            boost::system::error_code error = boost::asio::error::host_not_found;
            boost::asio::io_service io_service;
            boost::asio::ip::tcp::endpoint end_point( boost::asio::ip::address::from_string( host ), port );

            tcp::socket socket( io_service, end_point.protocol( ) );
            socket.connect( end_point, error);

            if (error)
                throw boost::system::system_error(error);

            socket.send( boost::asio::buffer( subscription ) );
            for (;;)
            {
                TcpMarketDataRecord r;
                socket.receive( boost::asio::buffer( &r, sizeof( r ) ) );

                r.timestampSecs = ntohl( r.timestampSecs );
                r.timestampMicrosecs = ntohl( r.timestampMicrosecs );

                const auto hours = r.timestampSecs / 3600;
                const auto minutes = ( r.timestampSecs % 3600 ) / 60;
                const auto secs = r.timestampSecs - hours * 3600 - minutes * 60;
                std::array<char, 5> temp { };
                std::copy( r.symbol, r.symbol + 4, temp.begin( ) );
                std::cout << hours << ':' << minutes << ':' << secs << '.' << r.timestampMicrosecs << ' '
                            << std::string( temp.data( ) ) << ' '
                            << ntohs( r.lastPriceInt ) << '.' << ntohs( r.lastPriceDec )
                            << std::endl;
            }
        }
        catch( boost::system::system_error & e ) {
            if ( e.code( ) != boost::asio::error::eof ) {
                std::cerr << e.what() << std::endl;
            }
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    // ======================================================================================================

}

