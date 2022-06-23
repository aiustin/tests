//
// Created by Iustin Amihaesei on 22/10/2020.
//

static std::string
b_not( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromBool( !op1.value().value<bool>() ) );
    return stack.fmt( stack.top( ) );
}

static std::string
b_and( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromBool( op1.value().value<bool>() && op2.value().value<bool>() ) );
    return stack.fmt( stack.top( ) );
}

static std::string
b_or( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromBool( op1.value().value<bool>() || op2.value().value<bool>() ) );
    return stack.fmt( stack.top( ) );
}

static std::string
b_xor( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromBool( op1.value().value<bool>() != op2.value().value<bool>() ) );
    return stack.fmt( stack.top( ) );
}


static std::string
b_ternary( OpStack & stack, Error & e )
{
    auto op3 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( op1.value( ).value<bool>() ? op2 : op3 );
    return stack.fmt( stack.top( ) );
}
