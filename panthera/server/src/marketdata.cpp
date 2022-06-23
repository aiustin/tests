#include "marketdata.h"


template<>
MarketData<DataMode::Mode_1>::MarketData() noexcept :
    _data( new MarketDataRecords( ) )
{
}

template<>
MarketData<DataMode::Mode_2>::MarketData() noexcept
{
}


template<> void
MarketData<DataMode::Mode_1>::insert( MarketDataRecord && r ) noexcept
{
    insert( *_data, std::move( r ) );
}


template<> void
MarketData<DataMode::Mode_2>::insert( MarketDataRecord && r ) noexcept
{
    auto iter = _data.find( r.symbol );
    if ( iter == _data.end( ) ) {
        auto [iter_insert, inserted] = _data.insert( { r.symbol, std::shared_ptr<MarketDataRecords>( new MarketDataRecords ) } );
        assert( inserted );
        insert( *iter_insert->second, std::move( r ) );
    }
    else {
        insert( *iter->second, std::move( r ) );
    }

}


/*static */ template<DataMode mode> void
MarketData<mode>::insert( std::vector<MarketDataRecord> & rs, MarketDataRecord && r ) noexcept
{
    auto iter = std::lower_bound( rs.begin( ), rs.end( ), r, []( const MarketDataRecord & r0, const MarketDataRecord & r1 ) {
        return r0.timestamp < r1.timestamp;
    } );
    rs.insert( iter, r );
}

std::ostream &
operator<<( std::ostream & os, const MarketDataRecords & rs )
{
    for( const auto & r : rs ) {
        os << r << std::endl;
    }
    return os;
}


template<> std::ostream &
operator<< <DataMode::Mode_1>( std::ostream & os, const MarketData<DataMode::Mode_1> & md )
{
    os << *md._data;
    return os;
}


template<> std::ostream &
operator<< <DataMode::Mode_2>( std::ostream & os, const MarketData<DataMode::Mode_2> & md )
{
    for( const auto & r : md._data ) {
        os << std::string( std::begin( r.first ), std::end( r.first ) ) << std::endl << *r.second << std::endl;
    }
    return os;
}
