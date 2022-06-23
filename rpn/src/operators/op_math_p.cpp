//
// Created by Iustin Amihaesei on 22/10/2020.
//


// Mathematic Functions


// Exponentiation
static std::string
math_exp( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromFloat( expf( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}

// Factorial
static std::string
math_fact( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    if ( ! op1.value( ).isInt( ) || op1.value( ).value<int>( ) < 0 ) {
        e = Error( ErrorType::E_InvalidNumericValue, "Don't know how to do that. Honestly!", { stack.fmt( op1 ) } );
        return std::string();
    }
    //TODO: implement it using ParallelPrimeSwing
    //TODO: add support for BigInt
    int fact = 1;
    for( auto i = 1; i < op1.value( ).value<int>( ); ++i ) {
        fact *= ( i + 1);
    }

    stack.push( Operand::fromInt( fact ) );
    return stack.fmt( stack.top( ) );
}

// Square Root
static std::string
math_sqrt( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromFloat( sqrtf( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}

// Natural Logarithm
static std::string
math_ln( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromFloat( logf( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}

// Base 10 logarithm
static std::string
math_log10( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromFloat( log10f( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}


// Base 2 logarithm
static std::string
math_log2( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromFloat( log2f( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}

// Raise a number to a power
static std::string
math_pow( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromFloat( powf( op1.value( ).value<float>( ), op2.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}


