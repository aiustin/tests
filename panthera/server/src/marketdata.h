#ifndef SERVER_MARKETDATA_H
#define SERVER_MARKETDATA_H

#include <string>
#include <chrono>
#include <unordered_map>
#include <fstream>
#include <boost/container_hash/hash.hpp>

#include "typedefs.h"
#include "marketdatarecord.h"

/**
 * The MarketData class provides the main interface for managing & loading
 * a set of MarketDataRecords
 *
 * In Mode_1:
 *  - all the records are stored sorted in a vector
 *
 * In Mode_2:
 *  - records are grouped in sorted vectors accessible through a hash map
 *
 * MarketData is read-only so data ca be passed around fast using shared_ptrs
 *
 */

#include <iostream>

template<DataMode mode>
struct MarketDataContainer {
    struct type {};
};

template<> struct MarketDataContainer<DataMode::Mode_1> { using type = MarketDataRecordsPtr; };
template<> struct MarketDataContainer<DataMode::Mode_2> { using type = std::unordered_map<MarketSymbol, MarketDataRecordsPtr, boost::hash<MarketSymbol>>; };

template<DataMode mode> class MarketData;
template<DataMode mode> std::ostream & operator<<( std::ostream & os, const MarketData<mode> & md );

template<DataMode mode>
class MarketData {
public:
    static MarketData fromCSV( const std::string & fileName );

    MarketData() noexcept;

    const typename MarketDataContainer<mode>::type & data( ) const noexcept {
        return _data;
    }

    template<typename T> typename std::enable_if<mode == DataMode::Mode_1, T>::type records( ) {
        return _data;
    }
    template<typename T> typename std::enable_if<mode == DataMode::Mode_2, T>::type records( const MarketSymbol & symbol ) {
        auto iter = _data.find( symbol );
        assert( iter != _data.end( ) );
        return iter->second;
    }

private:
    void insert( MarketDataRecord && r ) noexcept;
    static void insert( MarketDataRecords & rs, MarketDataRecord && r ) noexcept;

private:
    typename MarketDataContainer<mode>::type _data;

    friend std::ostream & operator<< <mode>( std::ostream & os, const MarketData<mode> & md );
};



/*static */ template<DataMode mode> MarketData<mode>
MarketData<mode>::fromCSV( const std::string & fileName ) {
    MarketData market_data;
    std::ifstream csv_file( fileName );

    if ( ! csv_file.is_open( ) ) {
        return market_data;
    }

    std::string line;
    while( std::getline( csv_file, line ) ) {
        auto r = MarketDataRecord::fromString( line );
        if ( r.valid ) {
            market_data.insert( std::move( r ) );
        }
    }

    return market_data;
}




#endif //SERVER_MARKETDATA_H
