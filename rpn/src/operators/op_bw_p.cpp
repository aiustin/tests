//
// Created by Iustin Amihaesei on 22/10/2020.
//

// Bitwise Operators
static std::string
bw_and( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromInt( op1.value().value<int>() & op2.value().value<int>() ) );
    return stack.fmt( stack.top( ) );
}

static std::string
bw_or( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromInt( op1.value( ).value<int>() | op2.value().value<int>() ) );
    return stack.fmt( stack.top( ) );
}

static std::string
bw_xor( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromInt( op1.value( ).value<int>() ^ op2.value( ).value<int>() ) );
    return stack.fmt( stack.top( ) );
}

static std::string
bw_not( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromInt( ~op1.value( ).value<int>() ) );
    return stack.fmt( stack.top( ) );
}

static std::string
bw_shl( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    if ( op2.value( ).value<int>() < 0 ) {
        e = Error( ErrorType::E_InvalidOperand, "No way I can shift that: ", { stack.fmt( op2 ) } );
        return std::string();
    }
    stack.push( Operand::fromInt( op1.value( ).value<int>() << op2.value( ).value<int>() ) );
    return stack.fmt( stack.top( ) );
}

static std::string
bw_shr( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    if ( op2.value( ).value<int>() < 0 ) {
        e = Error( ErrorType::E_InvalidOperand, "No way I can shift that: ", { stack.fmt( op2 ) } );
        return std::string();
    }
    stack.push( Operand::fromInt( op1.value( ).value<int>() >> op2.value( ).value<int>() ) );
    return stack.fmt( stack.top( ) );
}
