//
// Created by Iustin Amihaesei on 19/10/2020.
//

#include "operandvalue.h"

#include "util.h"

OperandValue::OperandValue(bool value ) :
        _type{ OpBool }
        ,   _value{ .bValue = value }
{

}


OperandValue::OperandValue(int value ) :
        _type{ OpInt }
        ,   _value{ .iValue = value }
{
}


OperandValue::OperandValue(float value ) :
        _type{ OpFloat }
        ,   _value{ .fValue = value }
{

}


OperandValue::OperandValue( const std::string & value ) :
    _type( OpString )
,   _strValue( value )
{

}


OperandValue::OperandValue( const std::vector<std::string> & value ) :
    _type( OpExpr )
,   _expr( value )
{

}



std::string
OperandValue::asString( ) const
{
    std::ostringstream oss;
    switch ( _type ) {
        case OpBool:
            oss << _value.bValue;
            break;
        case OpInt:
            oss << _value.iValue;
            break;
        case OpFloat:
            oss << _value.fValue;
            break;
        case OpString:
            return _strValue;
        case OpExpr:
            return Util::merge( _expr );
        case OpNone:
            break;

    }
    return oss.str();
}

Expresssion
OperandValue::asExpresssion() const
{
    return _type == OpExpr ? _expr : Expresssion( );
}


template<> bool
OperandValue::can_treat_as<float>() const
{
    return false;
}

template<> bool
OperandValue::can_treat_as<int>() const
{
    return _type == OpInt || _type == OpBool;
}




OperandValue
operator+(const OperandValue& op1, const OperandValue& op2 )
{
    return op1.can_treat_as<int>() && op2.can_treat_as<int>() ?
           OperandValue(op1.value<int>() + op2.value<int>() ) :
           OperandValue(op1.value<float>() + op2.value<float>() );
}

OperandValue
operator-(const OperandValue& op1, const OperandValue& op2 )
{
    return op1.can_treat_as<int>() && op2.can_treat_as<int>() ?
           OperandValue(op1.value<int>() - op2.value<int>() ) :
           OperandValue(op1.value<float>() - op2.value<float>() );
}

OperandValue
operator*(const OperandValue& op1, const OperandValue& op2 )
{
    return op1.can_treat_as<int>() && op2.can_treat_as<int>() ?
           OperandValue(op1.value<int>() * op2.value<int>() ) :
           OperandValue(op1.value<float>() * op2.value<float>() );
}

OperandValue
operator/(const OperandValue& op1, const OperandValue& op2 )
{
    return op1.can_treat_as<int>() && op2.can_treat_as<int>() ?
           OperandValue(op1.value<int>() / op2.value<int>() ) :
           OperandValue(op1.value<float>() / op2.value<float>() );
}

OperandValue
operator%(const OperandValue& op1, const OperandValue& op2 )
{
    assert( op1.can_treat_as<int>() && op2.can_treat_as<int>() );
    return OperandValue(op1.value<int>() % op2.value<int>() );
}

bool
operator==(const OperandValue& op1, const OperandValue& op2 )
{
    if ( op1.can_treat_as<int>() && op2.can_treat_as<int>() ) {
        return op1.value<int>() == op2.value<int>();
    }
    return op1.value<float>() == op2.value<float>();
}

bool
operator<(const OperandValue& op1, const OperandValue& op2 )
{
    if ( op1.can_treat_as<int>() && op2.can_treat_as<int>() ) {
        return op1.value<int>() < op2.value<int>();
    }
    return op1.value<float>() < op2.value<float>();
}


std::ostream&
operator<<( std::ostream & os, const OperandValue & value )
{
    os << value.asString( );
    return os;
}


