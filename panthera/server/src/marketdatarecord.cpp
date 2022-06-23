#include "marketdatarecord.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>


/*static */ MarketDataRecord
MarketDataRecord::fromString( const std::string & recordStr )
{
    auto r_str = recordStr;
    boost::trim( r_str );
    if ( r_str.empty( ) || r_str[ 0 ] == '#' ) {
        return MarketDataRecord( );
    }
    boost::to_upper( r_str );
    std::vector<std::string> tokens;
    boost::algorithm::split( tokens, r_str, boost::algorithm::is_any_of( "," ) );
    if ( tokens.size( ) != 3 ) {
        assert(false);
        return MarketDataRecord( );
    }
    return fromStringFields( tokens[ 1 ], tokens[ 0 ], tokens[ 2 ] );
}


/*static */ MarketDataRecord
MarketDataRecord::fromStringFields( const std::string & symbolStr, const std::string & timestampStr, const std::string & lastPriceStr )
{
    MarketDataRecord r;
    if ( symbolStr.empty( ) || symbolStr.size( ) > SymbolLength ) {
        assert( false );
        return r;
    }
    r.symbol.fill( 0 );
    std::copy( symbolStr.begin( ), symbolStr.end( ), r.symbol.begin( ) );
    std::vector<std::string> tokens;
    boost::algorithm::split( tokens, timestampStr, boost::algorithm::is_any_of( ":." ) );
    if ( tokens.size( ) != 4 ) {
        assert( false );
        return r;
    }

    r.timestamp = std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>(  );
    try {
        r.timestamp += std::chrono::hours( boost::lexical_cast<std::chrono::hours::rep>( tokens[ 0 ] ) );
        r.timestamp += std::chrono::minutes( boost::lexical_cast<std::chrono::minutes::rep>( tokens[ 1 ] ) );
        r.timestamp += std::chrono::seconds( boost::lexical_cast<std::chrono::seconds::rep>( tokens[ 2 ] ) );
        r.timestamp += std::chrono::microseconds( boost::lexical_cast<std::chrono::microseconds::rep>( tokens[ 3 ] ) );
    }
    catch ( const boost::bad_lexical_cast & ) {
        assert( false );
        return r;
    }

    tokens.clear( );
    boost::algorithm::split( tokens, lastPriceStr, boost::algorithm::is_any_of( "." ) );
    if ( tokens.size( ) == 1 ) {
        tokens.push_back( "0" );
    }
    if ( tokens.size( ) != 2 ) {
        assert( false );
        return r;
    }

    try {
        r.lastPriceInt = boost::lexical_cast<uint16_t>( tokens[ 0 ] );
        r.lastPriceDec = boost::lexical_cast<uint16_t>( tokens[ 1 ] );
    }
    catch ( const boost::bad_lexical_cast & ) {
        assert( false );
        return r;
    }
    r.valid = true;
    return r;
}


std::ostream &
operator<<( std::ostream & os, const MarketDataRecord & r )
{
    auto us = r.timestamp.time_since_epoch( );
    const auto hours = std::chrono::duration_cast<std::chrono::hours>( us );
    us -= std::chrono::duration_cast<std::chrono::microseconds>( hours );
    const auto minutes = std::chrono::duration_cast<std::chrono::minutes>( us );
    us -= std::chrono::duration_cast<std::chrono::microseconds>( minutes );
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>( us );
    us -= std::chrono::duration_cast<std::chrono::microseconds>( seconds );
    os << hours.count( ) << ':' << minutes.count( ) << ':' << seconds.count( ) << '.' << us.count( )
       << ' ' << std::string( std::begin( r.symbol ), std::end( r.symbol ) ) << ' ' << r.lastPriceInt << '.' << r.lastPriceDec;
    return os;
}
