//
// Created by Iustin Amihaesei on 18/10/2020.
//

#ifndef RPN_OPERATOR_H
#define RPN_OPERATOR_H

#include <string>
#include <unordered_map>
#include <functional>

#include "opstack.h"

class Operator
{
public:
    static std::string exec( OpStack & stack, const std::string & op, Error & err );
    static bool isOperator( const std::string & op ) { return _supportedOperators.find( op ) != _supportedOperators.end( ); }
private:
    using OperatorFn = std::function< std::string ( OpStack & stack, Error & e ) >;
    static const std::unordered_map< std::string, std::pair<OperatorFn, size_t> > _supportedOperators;
};


#endif //RPN_OPERATOR_H
