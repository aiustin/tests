//
// Created by Iustin Amihaesei on 20/10/2020.
//

#ifndef RPN_UTIL_H
#define RPN_UTIL_H

#include <string>
#include <vector>

namespace Util
{
    std::string toLower( const std::string & s );
    std::vector<std::string> split( const std::string & s );
    std::vector<std::string> mergeSubstr( const std::vector<std::string> & s, unsigned char delimiter = '`' );
    std::string merge( const std::vector<std::string> & ss, unsigned char delimiter = ' ' );
}

#endif //RPN_UTIL_H
