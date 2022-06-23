//
// Created by Iustin Amihaesei on 18/10/2020.
//

#ifndef RPN_ERROR_H
#define RPN_ERROR_H

#include <string>
#include <vector>

enum class ErrorType
{
    E_None,
    E_Quit,
    E_EmptyValue,
    E_InvalidNumericValue,
    E_OutOfRangeValue,
    E_InvalidValue,
    E_NumericOverflow,
    E_DivideByZero,
    E_InvalidOperand,
    E_NotEnoughOperands,
    E_VariableAlreadyDefined,
    E_InvalidFile,
};


struct Error
{
    Error( ) {}
    Error( ErrorType t, const std::string & m, const std::vector<std::string> & p = std::vector<std::string>( ) ) : type( t ), message( m ), params( p ) { }
    Error( const Error& ) = default;
    Error( Error&& ) = default;
    Error& operator=( const Error& ) = default;
    Error& operator=( Error&& ) = default;

    bool success() const { return type == ErrorType::E_None || quit( ) ; }
    bool quit( ) const  { return type == ErrorType::E_Quit; }

    ErrorType type = ErrorType::E_None;
    std::string message;
    std::vector<std::string> params;
};


#endif //RPN_ERROR_H
