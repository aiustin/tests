#ifndef SERVER_MARKETDATARECORD_H
#define SERVER_MARKETDATARECORD_H

#include <string>
#include <chrono>
#include <vector>
#include <array>
#include <memory>


/**
 * The MarketDataRecord class provides the core functionality for managing a market data record
 *
 * Fields:
 *  - Symbol: 4 chars
 *  - Timestamp
 *  - Last price
 *  - valid: is false when deserialization fails
 */


inline constexpr std::size_t SymbolLength = 4;
using MarketSymbol = std::array<char, SymbolLength>;
using MarketTimestamp = std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>;

struct MarketDataRecord {
    MarketSymbol symbol { };
    MarketTimestamp timestamp;
    uint16_t lastPriceInt;
    uint16_t lastPriceDec;
    bool valid = false;

    static MarketDataRecord fromString( const std::string & recordStr );
    static MarketDataRecord fromStringFields( const std::string & symbolStr, const std::string & timestampStr, const std::string & lastPriceStr );
};

using MarketDataRecords = std::vector<MarketDataRecord>;
using MarketDataRecordsPtr = std::shared_ptr<MarketDataRecords>;

std::ostream & operator<<( std::ostream & os, const MarketDataRecord & r );


#endif //SERVER_MARKETDATARECORD_H
