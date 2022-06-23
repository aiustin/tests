//
// Created by Iustin Amihaesei on 20/10/2020.
//

#include "util.h"

#include <algorithm>
#include <cctype>



namespace Util {

    std::string
    toLower( const std::string & s )
    {
        auto lower_str = s;
        std::transform( lower_str.begin( ), lower_str.end( ), lower_str.begin( ), []( unsigned  char c ) { return std::tolower( c ); } );
        return lower_str;
    }


    std::vector<std::string>
    split( const std::string & s )
    {
        std::vector<std::string> result;
        std::string term;
        for( auto i : s )
        {
            if ( ! std::isspace( i ) ) {
                term.push_back( i );
            }
            else if ( term.length( ) ) {
                result.push_back( term );
                term.clear();
            }
        }
        if ( term.length( ) ) {
            result.push_back( term );
        }
        return result;
    }

    std::vector<std::string>
    mergeSubstr( const std::vector<std::string> & s, unsigned char delimiter /*= '`'*/ )
    {
        std::vector<std::string> merged;
        std::string merging;
        for( auto i = 0; i < s.size( ); ++i ) {
            if( s[ i ].empty( ) ) {
                continue;
            }
            // TODO: Improve parsing
            if ( s[ i ][ 0 ] == delimiter ) {
                if ( merging.empty( ) ) {
                    merging = s[ i ];
                }
                else {
                    merging += ' ' + s[ i ];
                    merged.push_back( merging );
                    merging.clear( );
                }
            }
            else if ( ! merging.empty( ) ) {
                merging += ' ' + s[ i ];
            }
            else {
                merged.push_back( s[ i ] );
            }
            if ( merging.size( ) > 1 && merging[ merging.size( ) - 1 ] == delimiter ) {
                merged.push_back( merging );
                merging.clear( );
            }
        }
        return merged;
    }

    std::string
    merge( const std::vector<std::string> & ss, unsigned char delimiter /*= ' '*/ )
    {
        std::string s;
        for( auto i = 0; i < ss.size( ); ++i ) {
            if (ss[i].empty()) {
                continue;
            }
            if ( i ) {
                s += delimiter;
            }
            s += ss[ i ];
        }
        return s;
    }

}