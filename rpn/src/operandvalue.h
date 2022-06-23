//
// Created by Iustin Amihaesei on 19/10/2020.
//

#ifndef RPN_OPERANDVALUE_H
#define RPN_OPERANDVALUE_H

#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>


using Expresssion = std::vector<std::string>;

class OperandValue
{
public:
    enum OpType
    {
        OpNone,
        OpBool,
        OpInt,
        OpFloat,
        OpString,
        OpExpr
    };

public:
    OperandValue() {}
    explicit OperandValue( bool value );
    explicit OperandValue( int value );
    explicit OperandValue( float value );
    explicit OperandValue( const std::string & value );
    explicit OperandValue( const std::vector<std::string> & value );

    OperandValue(const OperandValue & ) = default;
    OperandValue(OperandValue && ) = default;
    OperandValue& operator=(const OperandValue & ) = default;
    OperandValue& operator=(OperandValue && ) = default;
    ~OperandValue() = default;

    template<typename T> T value() const;
    std::string asString() const;
    Expresssion asExpresssion() const;
    template<typename T> bool can_treat_as() const;

    bool isValid( ) const { return  _type != OpNone; }
    bool isInt( ) const { return _type == OpInt; }
    bool isFloat( ) const { return _type == OpFloat; }
    bool isBool( ) const { return _type == OpBool; }
    bool isString( ) const { return _type == OpString; }
    bool isNumeric( ) const { return ! isString( ) && ! isExpression( ); }
    bool isExpression( ) const { return _type == OpExpr;}

private:
    // Could probably use std::variant in C++17

    OpType _type = OpNone;
    union
    {
        bool bValue;
        int iValue;
        float fValue;
    } _value;
    std::string _strValue;
    Expresssion _expr;

    friend OperandValue operator+(const OperandValue& op1, const OperandValue& op2 );
    friend OperandValue operator-(const OperandValue& op1, const OperandValue& op2 );
    friend OperandValue operator*(const OperandValue& op1, const OperandValue& op2 );
    friend OperandValue operator/(const OperandValue& op1, const OperandValue& op2 );
    friend OperandValue operator%(const OperandValue& op1, const OperandValue& op2 );
    friend bool operator==(const OperandValue& op1, const OperandValue& op2 );
    friend bool operator!=(const OperandValue& op1, const OperandValue& op2 );
    friend bool operator<(const OperandValue& op1, const OperandValue& op2 );
    friend bool operator<=(const OperandValue& op1, const OperandValue& op2 );
    friend bool operator>(const OperandValue& op1, const OperandValue& op2 );
    friend bool operator>=(const OperandValue& op1, const OperandValue& op2 );

    friend std::ostream& operator<<( std::ostream & os, const OperandValue & value );
};

OperandValue operator+(const OperandValue& op1, const OperandValue& op2 );
OperandValue operator-(const OperandValue& op1, const OperandValue& op2 ); // Can be implemented as op1 + (-op2)
OperandValue operator*(const OperandValue& op1, const OperandValue& op2 );
OperandValue operator/(const OperandValue& op1, const OperandValue& op2 );
OperandValue operator%(const OperandValue& op1, const OperandValue& op2 );

bool operator==(const OperandValue& op1, const OperandValue& op2 );
inline bool operator!=(const OperandValue& op1, const OperandValue& op2 ) { return !(op1 == op2); }
bool operator<(const OperandValue& op1, const OperandValue& op2 );
inline bool operator<=(const OperandValue& op1, const OperandValue& op2 ) { return (op1 < op2 || op1 == op2); }
inline bool operator>(const OperandValue& op1, const OperandValue& op2 ) { return !(op1 <= op2 ); }
inline bool operator>=(const OperandValue& op1, const OperandValue& op2 ) { return !(op1 < op2 ); }


#define OPER_ADD 1
#define OPER_SUB 2
#define OPER_MUL 3
#define OPER_DIV 4

#if 0
template<typename T>
bool overflow( const OperandValue & op, T value, int oper )
{
    assert( op.isNumeric( ) );
    switch ( oper ) {
        case OPER_ADD:
            return  (op.value<T>() > 0 && value > 0 && std::numeric_limits<T>::max() - op.value<T>() > value ) ||
                    (op.value<T>() < 0 && value < 0 && std::numeric_limits<T>::min() - op.value<T>() > value )
                    ;
        case OPER_SUB:
            return  (op.value<T>() > 0 && value < 0 && std::numeric_limits<T>::max() - op.value<T>() > -value ) ||
                    (op.value<T>() < 0 && value > 0 && std::numeric_limits<T>::min() - op.value<T>() > -value )
                    ;
        case OPER_DIV:
            return value < 1 && value > -1 &&
                    ( ( ( op.value<T>() > 0 && value > 0 ) || ( op.value<T>() < 0 && value < 0 ) )  && op.value<T>() > std::numeric_limits<T>::max() * value ) ||
                    ( ( op.value<T>() < 0 && value > 0 ) && op.value<T>() < std::numeric_limits<T>::min() * value ) ||
                    ( ( op.value<T>() > 0 && value < 0 ) && op.value<T>() > std::numeric_limits<T>::min() * value )
                    ;
        case OPER_MUL:
            return  (value != 0 && std::numeric_limits<T>::max() / std::abs( value ) > op.value<T>() )
                    ;
            break;
    }
}
#endif
template<typename T>
bool overflow( T op, T value, int oper )
{
    switch ( oper ) {
        case OPER_ADD:
            return  (op > 0 && value > 0 && std::numeric_limits<T>::max() - op < value ) ||
                    (op < 0 && value < 0 && std::numeric_limits<T>::min() - op > value )
                    ;
            // Can be implemented as OPER_ADD (- value)
        case OPER_SUB:
            return  (op > 0 && value < 0 && std::numeric_limits<T>::max() - op < -value ) ||
                    (op < 0 && value > 0 && std::numeric_limits<T>::min() - op > -value )
                    ;
        case OPER_DIV:
            // Can be implemented as OPER_MUL (1/value if value != 0)
            return value < 1 && value > -1 &&
                   ( ( ( op > 0 && value > 0 ) || ( op < 0 && value < 0 ) )  && op > std::numeric_limits<T>::max() * value ) ||
                   ( ( op < 0 && value > 0 ) && op < std::numeric_limits<T>::min() * value ) ||
                   ( ( op > 0 && value < 0 ) && op > std::numeric_limits<T>::min() * value )
                    ;
        case OPER_MUL:
            return  (value != 0 && std::numeric_limits<T>::max() / std::abs( value ) < op )
                    ;
            break;
        default:
            assert(false);
            break;
    }
    return false;
}

template<typename T> T
OperandValue::value() const
{
    switch ( _type ) {
        case OpBool:
            return static_cast<T>( _value.bValue );
        case OpInt:
            return static_cast<T>( _value.iValue );
        case OpFloat:
            return static_cast<T>( _value.fValue );
        case OpExpr:
        case OpString:
            //TODO: add support for string to numeric if necessary
        case OpNone:
            assert( false );
            break;

    }
    return 0.0;
}



#endif //RPN_OPERANDVALUE_H
