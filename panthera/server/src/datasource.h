#ifndef SERVER_DATASOURCE_H
#define SERVER_DATASOURCE_H


#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include <atomic>

#include "typedefs.h"
#include "marketdatarecord.h"

/**
 * The DataSource class provides records to a consumer
 *  - though there is nothing to stop it being copyable move semantics is much more appropriate for it
 *
 * In Mode_1:
 *  - records are fetched in a tight loop comparing timestamps to now()
 *  - findFirst positions finds the first record >= now( )
 *
 * In Mode_2:
 *  - records are fetched one by one for each call
 *  - findFirst uses a custom timestamp
 *
 *
 */

template<DataMode>
class DataSource {
public:

public:
    DataSource( const MarketDataRecordsPtr & rs,
                const MarketTimestamp & ts = MarketTimestamp( std::chrono::duration_cast<std::chrono::microseconds>( std::chrono::system_clock::now( ).time_since_epoch( ) ) ) ) noexcept :
                _records( rs ),
                _startTimestamp( ts )
                { }
    ~DataSource( ) noexcept = default;
    DataSource( const DataSource & ) =delete;
    DataSource( DataSource && ) noexcept =default;
    DataSource & operator=( const DataSource & ) =delete;
    DataSource & operator=( DataSource && ) noexcept =default;

    void findFirst( ) noexcept;
    bool fetchNext( MarketDataRecord & r, const std::atomic_bool & terminate ) noexcept;

private:
    void findFirst( const MarketTimestamp & timestamp ) noexcept;

private:
    MarketDataRecordsPtr _records;
    MarketTimestamp _startTimestamp;
    int _nextRecord = 0;
};



template<DataMode mode> void
DataSource<mode>::findFirst( const MarketTimestamp & timestamp ) noexcept {
    auto iter = std::lower_bound( _records->begin( ), _records->end( ), timestamp,
                                  []( const MarketDataRecord & r0, const MarketTimestamp & value ) {
                                      return r0.timestamp < value;
                                  }
    );
    _nextRecord = iter - _records->begin( );
}


#endif //SERVER_DATASOURCE_H
