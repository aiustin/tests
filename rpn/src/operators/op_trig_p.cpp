//
// Created by Iustin Amihaesei on 22/10/2020.
//


static std::string
trig_acos( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromFloat( acosf( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}

static std::string
trig_asin( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromFloat( asinf( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}

static std::string
trig_atan( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromFloat( atanf( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}

static std::string
trig_cos( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromFloat( cosf( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}

static std::string
trig_cosh( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromFloat( coshf( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}


static std::string
trig_sin( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromFloat( sinf( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}


static std::string
trig_sinh( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromFloat( sinhf( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}


static std::string
trig_tanh( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    stack.push( Operand::fromFloat( tanhf( op1.value( ).value<float>( ) ) ) );
    return stack.fmt( stack.top( ) );
}
