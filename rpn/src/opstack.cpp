//
// Created by Iustin Amihaesei on 18/10/2020.
//

#include "opstack.h"

#include "operator.h"
#include "util.h"

#include <bitset>
#include <math.h>

/*static */ std::string
OpStack::fmt( int value, int base )
{
    std::ostringstream fmt_str;
    if ( base == 2 ) {
        std::bitset<sizeof(int)> bin(value);
        fmt_str << "0b" << bin;
    }
    else {
        fmt_str << ( base == 16 ? std::hex :
                        ( base == 8 ? std::oct : std::dec )
                    )
                    << std::showbase << value;
    }
    return fmt_str.str( );
}


std::string
OpStack::push( const std::string & value, Error * err /*= nullptr*/ )
{
    assert( value.length( ) );
    Error e;
    if ( ! value.length( ) )
    {
        if ( err ) {
            e = Error{ ErrorType::E_EmptyValue, "Empty value" };
        }
    }
    else
    {
        // TODO: remove hard-coded values; make these static constants (or configurable)
        if ( value[ 0 ] == '@' || value[ 0 ] == '#' || value[ 0 ] == '`' ) {
            if ( value[ 0 ] == '@' )
            {
                Operand op;
                auto i = _vars.find( value );
                if( i == _vars.end( ) ) {
                    op = Operand( value, OperandValue( 0 ) );
                    _vars[ value ] = op;
                }
                else {
                    op = i->second;
                }
                push( op );
            }
            else if ( value[ 0 ] == '#' )
            {
                Operand op;
                auto i = _macros.find( value );
                if( i == _macros.end( ) ) {
                    op = Operand( value, OperandValue( 0 ) );
                    _macros[ value ] = op;
                }
                else {
                    op = i->second;
                }
                push( op );
            }
            else if ( value[ 0 ] == '`' ) {
                if ( value.length( ) < 3 || value[ value.size( ) - 1 ] != '`' ) {
                    e = Error{ ErrorType::E_InvalidValue, "Empty or invalid raw value." };
                }
                else {
                    const auto expr = Util::mergeSubstr( Util::split( Util::toLower( value.substr( 1, value.size( ) -  2 ) ) ) );
                    if ( expr.empty( ) ) {
                        e = Error{ ErrorType::E_InvalidValue, "Empty expression." };
                    }
                    else {
                        push( Operand::fromExpr( expr ) );
                    }
                }
            }
        }
        else if ( value[ 0 ] == '"' ) {
            if ( value.length( ) < 3 || value[ value.size( ) - 1 ] != '"' ) {
                e = Error{ ErrorType::E_InvalidValue, "Empty or invalid string value." };
            }
            else {
                push( Operand::fromString( value.substr( 1, value.length( ) - 2 ) ) );
            }
        }
        else {
            static const std::string cst_pi("pi");
            static const std::string cst_e("e");
            static const std::string cst_rand("rand");
            static const std::string cst_true("true");
            static const std::string cst_false("false");
            static const std::string cst_true_short("t");
            static const std::string cst_false_short("f");
            if  (value == cst_true || value == cst_true_short ) {
                push( Operand::fromBool( true ) );
            }
            else if  (value == cst_false || value == cst_false_short ) {
                push( Operand::fromBool( false ) );
            }
            else if (value == cst_pi) {
                // C++17/20 has new constants
                push( Operand::fromFloat( (float) M_PI) );
            } else if (value == cst_e) {
                // C++17/20 has new constants
                push( Operand::fromFloat( ( float) M_E) );
            } else if (value == cst_rand) {
                push( Operand::fromInt( rand( ) ) );
            } else {
                size_t idx = 0;
                try {
                    try{
                        int iValue = 0;
                        float fValue = 0.0;
                        std::string conv_value = value;
                        if ( value.length( ) >= 2 && value[ 0 ] == '0' && value[ 1 ] == 'b' ) {
                            conv_value = value.substr( 2, value.length() - 2);
                            iValue = std::stoi( conv_value, &idx, 2 );
                        }
                        else {
                            iValue = std::stoi( conv_value, &idx, 0 );
                        }

                        if ( idx != conv_value.length( ) ) {
                            fValue = std::stof( value, &idx );
                            if ( idx != value.length( ) ) {
                                e = Error{ErrorType::E_InvalidNumericValue, "Invalid numeric value:", { value } };
                            }
                            else {
                                push( Operand::fromFloat( fValue ) );
                            }
                        }
                        else {
                            push( Operand::fromInt( iValue ) );
                        }
                    }
                    catch (const std::out_of_range &) {
                        e = Error{ErrorType::E_OutOfRangeValue, "Out of range value: ", { value } };
                    }
                }
                catch (const std::invalid_argument &) {
                    e = Error{ErrorType::E_InvalidNumericValue, "Invalid numeric value:", { value } };
                }
            }
        }
    }
    if ( err ) {
        *err = e;
    }

    return  e.success() ? echo( fmt( top( ) ) ) : std::string( );
}

void
OpStack::push( const Operand & value )
{
    _stack.push_back( value);
}

void
OpStack::assign( Operand op, OperandValue value )
{
    op.setValue( value );
    if ( op.isVar( ) ) {
        _vars[ op.var( ) ].setValue( op.value( ) );
    }
    else if ( op.isMacro( ) ) {
        _macros[ op.var( ) ].setValue( op.value( ) );
    }
}

Operand
OpStack::top( ) const
{
    return isEmpty( ) ? Operand( ) : _stack[ _stack.size( ) - 1 ];
}

Operand
OpStack::pop( )
{
    if ( isEmpty( ) ) {
        return Operand( );
    }
    Operand op = top( );
    _stack.pop_back( );
    return op;
}

Operand
OpStack::pop( Error & e )
{
    auto op = pop( );
    eval( op, e );
    return op;
}

Operand
OpStack::popNumeric( Error & e )
{
    auto op = pop( e );
    if ( e.success( ) ) {
        if ( ! op.value( ).isNumeric( ) ) {
            e = Error( ErrorType::E_InvalidNumericValue, "Arghh.. I need a numeric one, not this:", { fmt( op ) } );
        }
    }
    return op;
}

Operand
OpStack::popString( Error & e )
{
    auto op = pop( e );
    if ( e.success( ) ) {
        if ( ! op.value( ).isString( ) ) {
            e = Error( ErrorType::E_InvalidOperand, "You're trying to trick me! This is not a string:", { fmt( op ) } );
        }
    }
    return op;
}


std::string
OpStack::fmt( const Operand & op )
{
    auto op_value = op.value( );
    if ( op_value.isInt( ) ) {
        return fmt( op_value.value<int>(), _state.base );
    }
    else if ( op_value.isBool( ) ) {
        // TODO: Could use locale
        return op_value.value<bool>() ? "True" : "False";
    }
    return op_value.asString();
}

std::string
OpStack::fmt( )
{
    std::ostringstream oss;
    oss << *this;
    return oss.str( );
}


bool
OpStack::evalExpression( Operand & op, Error & e, std::string * output /*= nullptr*/ )
{
    //TODO: add meaningfull error messages and propagate it to the caller
    std::string o;

    if ( ! eval( op.value( ).asExpresssion( ), e, o ) ) {
        op = Operand( );
    }
    else {
        if( _stack.empty( ) ) {
            op = Operand(  );
        }
        else {
            op.setValue( _stack[ _stack.size( ) - 1 ].value( ) );
            _stack.pop_back( );
        }
    }
    if( output ) {
        *output = o;
    }
    return true;
}

bool
OpStack::eval( Operand & op, Error & e, std::string * output /* = nullptr*/ )
{
    if ( ! op.isValid( ) ) {
        e = Error{ ErrorType::E_InvalidOperand, "Sorry. There's nothing valid I can work with." };
        return false;
    }
    if ( !op.isVar( ) ) {
        if ( op.value( ).isExpression( ) ) {
            return evalExpression( op, e, output );
        }
    }
    else {
        auto i = _vars.find( op.var( ) );
        assert( i != _vars.end( ) );
        if ( i != _vars.end( ) ) {
            if( i->second.value( ).isExpression( ) ) {
                return evalExpression( op, e, output );
            }
            else {
                op.setValue( i->second.value( ) );
            }
        }
    }
    return true;
}


bool
OpStack::eval( const std::string & expr, Error & e, std::string & output )
{
    auto value = preprocess( Util::toLower( expr ) );
    return eval( Util::mergeSubstr( Util::split( value ) ), e, output );
}


bool
OpStack::eval( const std::vector<std::string> & expr, Error & e, std::string & output )
{
    bool result = true;
    for( const auto & i : expr )
    {
        if ( Operator::isOperator( i ) ) {
            output = Operator::exec( *this, i, e );
        }
        else {
            output = push( i, &e );
        }
        if ( e.quit( ) ) {
            break;
        }
        else if ( ! e.success( ) ) {
            result = false;
            break;
        }
    }
    return result;
}


std::string
OpStack::preprocess( const std::string & v )
{
    auto value = v;
    for( const auto & m : _macros ) {
        const std::string replace_value = ' ' + m.second.value().asString( ) + ' ';
        const std::string search_value = m.second.var( ).substr( 1, m.second.var( ).length( ) - 1);
        std::string::size_type pos = 0;
        auto i = 0;
        std::string new_value;
        while ( ( pos = value.find( search_value, pos ) ) != std::string::npos ) {
            new_value += value.substr( i, pos - i ) + replace_value;
            pos += search_value.length( );
            i = pos;
        }
        new_value += value.substr( i, pos - i );
        value = new_value;
    }
    return value;
}


std::ostream&
operator<<( std::ostream & os, const OpStack & stack )
{
    // TODO: Don't abuse the skipws flag any more
    os.setf( os.skipws );
    if ( stack._state.displayVertically ) {
        os << "--- Values ---" << std::endl;
        std::for_each( stack._stack.begin( ), stack._stack.end( ), [&os]( const Operand & i) -> void { os << i << std::endl; } );
    }
    else {
        os << "--- Values ---:" << std::endl;
        std::for_each( stack._stack.begin( ), stack._stack.end( ), [&os]( const Operand & i) -> void { os << i << " "; } );
        os << std::endl;
    }
    os.unsetf( os.skipws );
    if ( stack._state.displayVertically ) {
        os << "--- Variables ---" << std::endl;
        std::for_each( stack._vars.begin( ), stack._vars.end( ), [&os]( const std::unordered_map<std::string, Operand>::value_type & i) -> void { os << i.second << std::endl; } );
    }
    else {
        os << "--- Variables ---:" << std::endl;
        std::for_each( stack._vars.begin( ), stack._vars.end( ), [&os]( const std::unordered_map<std::string, Operand>::value_type & i) -> void { os << i.second << " "; } );
        os << std::endl;
    }
    if ( stack._state.displayVertically ) {
        os << "--- Macros ---"  << std::endl;
        std::for_each( stack._macros.begin( ), stack._macros.end( ), [&os]( const std::unordered_map<std::string, Operand>::value_type & i) -> void { os << i.second << std::endl; } );
    }
    else {
        os << "--- Macros ---:" << std::endl;
        std::for_each( stack._macros.begin( ), stack._macros.end( ), [&os]( const std::unordered_map<std::string, Operand>::value_type & i) -> void { os << i.second << " "; } );
        os << std::endl;
    }

    return os;
}

