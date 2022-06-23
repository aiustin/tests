#include "datasource.h"

//#define TESTING_IN_PROGRESS

template<> bool
DataSource<DataMode::Mode_1>::fetchNext( MarketDataRecord & r, const std::atomic_bool & terminate ) noexcept {
    const auto & symbol_data = *_records;
    if ( _nextRecord >= symbol_data.size( ) ) {
        return false;
    }
    r = symbol_data[ _nextRecord++ ];

    const auto end = r.timestamp
#ifdef TESTING_IN_PROGRESS
	- std::chrono::hours( 17 ) - std::chrono::minutes( 0 )
#endif
	;
    const auto end_today = end + std::chrono::duration_cast<std::chrono::duration<int64_t, std::ratio<86400>>>( std::chrono::system_clock::now( ).time_since_epoch( ) );
    //TODO: on some older version of glibc system_clock::now() is implemented with system calls
    // it may be to slow anyway, so maybe a steady_clock would be better
    // On mac this goes to mach_absolute_time which is very expensive. Experiment with steady_clock, or some other solution
    while ( std::chrono::system_clock::now( ) < end_today && ! terminate ) {
        // Not much point in wasting time yielding, plus we might miss the timestamp
        //std::this_thread::yield( );
    }
    return ! terminate;
}


template<> bool
DataSource<DataMode::Mode_2>::fetchNext( MarketDataRecord & r, const std::atomic_bool & /* terminate */ ) noexcept {
    if ( _nextRecord >= _records->size( ) ) {
        return false;
    }

    r = (*_records)[ _nextRecord++ ];
    return true;
}


template<> void
DataSource<DataMode::Mode_1>::findFirst( ) noexcept
{
    const auto now = std::chrono::system_clock::now( )
#ifdef TESTING_IN_PROGRESS
	+ std::chrono::hours( 17 ) + std::chrono::minutes( 0 )
#endif
	;
    const auto today = std::chrono::duration_cast<std::chrono::duration<int64_t, std::ratio<86400>>>( now.time_since_epoch( ) );
    const MarketTimestamp first_timestamp_us( std::chrono::duration_cast<std::chrono::microseconds>( now.time_since_epoch( ) - today ) );
    findFirst( first_timestamp_us );
}


template<> void
DataSource<DataMode::Mode_2>::findFirst( ) noexcept
{
    // publish all data
    findFirst( _startTimestamp );
}



