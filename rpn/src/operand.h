//
// Created by Iustin Amihaesei on 18/10/2020.
//

#ifndef RPN_OPERAND_H
#define RPN_OPERAND_H

#include "operandvalue.h"

#include <string>
#include <vector>

class Operand
{
public:
    Operand( ) { }
    explicit Operand(const OperandValue & value ) : _value( value ) { }
    explicit Operand(const std::string & var, const OperandValue & value ) : _var( var ), _value( value ) { }

    static Operand fromBool( bool value ) { return Operand( OperandValue( value ) ); }
    static Operand fromInt( int value ) { return Operand( OperandValue( value ) ); }
    static Operand fromFloat( float value ) { return Operand( OperandValue( value ) ); }
    static Operand fromString( const std::string & value ) { return Operand( OperandValue( value ) ); }
    static Operand fromExpr( const std::vector<std::string> & value ) { return Operand( OperandValue( value ) ); }

    const OperandValue & value( ) const { return _value; }
    const std::string & var( ) const { return _var; }
    void setValue( const OperandValue & value ) { _value = value; }

    bool isVar() const { return !_var.empty( ) && _var[0] == '@'; }
    bool isMacro() const { return !_var.empty( ) && _var[0] == '#'; }
    bool isValid( ) const { return _value.isValid( ); }

private:
    std::string _var;
    OperandValue _value;

    friend std::ostream& operator<<( std::ostream & os, const Operand & op );
};


inline Operand operator+(const Operand& op1, const Operand& op2 ) { return Operand( op1.value( ) + op2.value( ) ); }
inline Operand operator-(const Operand& op1, const Operand& op2 ) { return Operand( op1.value( ) - op2.value( ) ); }
inline Operand operator*(const Operand& op1, const Operand& op2 ) { return Operand( op1.value( ) * op2.value( ) ); }
inline Operand operator/(const Operand& op1, const Operand& op2 ) { return Operand( op1.value( ) / op2.value( ) ); }
inline Operand operator%(const Operand& op1, const Operand& op2 ) { return Operand( op1.value( ) % op2.value( ) ); }

inline bool operator==(const Operand& op1, const Operand& op2 ) { return op1.value() == op2.value(); }
inline bool operator!=(const Operand& op1, const Operand& op2 ) { return op1.value() != op2.value(); }
inline bool operator<(const Operand& op1, const Operand& op2 ) { return op1.value() < op2.value(); }
inline bool operator<=(const Operand& op1, const Operand& op2 ) { return op1.value() <= op2.value(); }
inline bool operator>(const Operand& op1, const Operand& op2 ) { return op1.value() > op2.value(); }
inline bool operator>=(const Operand& op1, const Operand& op2 ) { return op1.value() < op2.value(); }


#endif //RPN_OPERAND_H
