//
// Created by Iustin Amihaesei on 22/10/2020.
//




static std::string
var_assign( OpStack & stack, Error & e )
{
    auto op2 = stack.popRaw( );
    auto op1 = stack.popRaw( );
    stack.assign( op1, op2.value( ) );
    return std::string( );
}

static std::string
var_eval( OpStack & stack, Error & e )
{
    auto op1 = stack.pop( e );
    stack.push( op1.isVar( ) ? Operand( op1.value( ) ) : op1 );
    return stack.fmt( op1 );
}

