//
// Created by Iustin Amihaesei on 22/10/2020.
//


// Numeric Utilities

static std::string
num_ceil( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromInt( ceilf( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}

static std::string
num_floor( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromInt( floorf( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}

static std::string
num_round( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromInt( roundf( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}

static std::string
num_ip( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    float intpart;
    std::modf( op1.value( ).value<float>( ), &intpart );
    stack.push( Operand::fromInt( (int) intpart ) );
    return stack.fmt( stack.top( ) );
}

static std::string
num_fp( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    float intpart;
    float floatpart = std::modf( op1.value( ).value<float>( ), &intpart );
    stack.push( Operand::fromFloat( floatpart ) );
    return stack.fmt( stack.top( ) );
}

static std::string
num_sign( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromInt( ( 0.0f < op1.value( ).value<float>( ) ) - (op1.value( ).value<float>( ) < 0.0f ) ) );
    return stack.fmt( stack.top( ) );
}

static std::string
num_abs( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    if ( op1.value( ).isFloat( ) ) {
        stack.push( Operand::fromFloat( fabsf( op1.value( ).value<float>( ) ) ) );
    }
    else {
        stack.push( Operand::fromInt( abs( op1.value( ).value<int>( ) ) ) );
    }
    return stack.fmt( stack.top( ) );
}

static std::string
num_max( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    if ( op1.value( ).isFloat( ) || op2.value( ).isFloat( ) ) {
        stack.push( Operand::fromFloat( std::max( op1.value( ).value<float>( ), op2.value( ).value<float>( ) ) ) );
    }
    else {
        stack.push( Operand::fromInt( std::max( op1.value( ).value<int>( ),op2.value( ).value<int>( ) ) ) );
    }
    return stack.fmt( stack.top( ) );
}

static std::string
num_min( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    if ( op1.value( ).isFloat( ) || op2.value( ).isFloat( ) ) {
        stack.push( Operand::fromFloat( std::min( op1.value( ).value<float>( ), op2.value( ).value<float>( ) ) ) );
    }
    else {
        stack.push( Operand::fromInt( std::min( op1.value( ).value<int>( ),op2.value( ).value<int>( ) ) ) );
    }
    return stack.fmt( stack.top( ) );
}

