//
// Created by Iustin Amihaesei on 22/10/2020.
//


// Comparison Operators

static std::string
comp_lt( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromBool( op1 < op2 ) );
    return stack.fmt( stack.top( ) );
}

static std::string
comp_le( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromBool( op1 <= op2 ) );
    return stack.fmt( stack.top( ) );
}

static std::string
comp_e( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromBool( op1 == op2 ) );
    return stack.fmt( stack.top( ) );
}

static std::string
comp_ne( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromBool( op1 != op2 ) );
    return stack.fmt( stack.top( ) );
}

static std::string
comp_gt( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromBool( op1 > op2 ) );
    return stack.fmt( stack.top( ) );
}

static std::string
comp_ge( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromBool( op1 >= op2 ) );
    return stack.fmt( stack.top( ) );
}

