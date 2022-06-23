//
// Created by Iustin Amihaesei on 22/10/2020.
//

// Arithmetic Operators

static std::string
arithm_plus( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }

    if ( op1.value( ).can_treat_as<int>() && op2.value( ).can_treat_as<int>() )
    {
        if ( overflow( op1.value( ).value<int>(), op2.value( ).value<int>(), OPER_ADD ) ) {
            e = Error( ErrorType::E_NumericOverflow, "I really can't do it. I'm gonna overflow with these:", { stack.fmt( op1 ), stack.fmt( op2 ) } );
            return std::string();
        }
    }
    else {
        if ( overflow( op1.value( ).value<float>(), op2.value( ).value<float>(), OPER_ADD ) ) {
            e = Error( ErrorType::E_NumericOverflow, "I really can't do it. I'm gonna overflow with these:", { stack.fmt( op1 ), stack.fmt( op2 ) } );
            return std::string();
        }
    }
    stack.push( op1 + op2 );
    return stack.fmt( stack.top( ) );
}


static std::string
arithm_minus( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    if ( op1.value( ).can_treat_as<int>() && op2.value( ).can_treat_as<int>() )
    {
        if ( overflow( op1.value( ).value<int>(), op2.value( ).value<int>(), OPER_SUB ) ) {
            e = Error( ErrorType::E_NumericOverflow, "I really can't do it. I'm gonna overflow with these:", { stack.fmt( op1 ), stack.fmt( op2 ) } );
            return std::string();
        }
    }
    else {
        if ( overflow( op1.value( ).value<float>(), op2.value( ).value<float>(), OPER_SUB ) ) {
            e = Error( ErrorType::E_NumericOverflow, "I really can't do it. I'm gonna overflow with these:", { stack.fmt( op1 ), stack.fmt( op2 ) } );
            return std::string();
        }
    }
    stack.push( op1 - op2 );
    return stack.fmt( stack.top( ) );
}


static std::string
arithm_mul( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    if ( op1.value( ).can_treat_as<int>() && op2.value( ).can_treat_as<int>() )
    {
        if ( overflow( op1.value( ).value<int>(), op2.value( ).value<int>(), OPER_MUL ) ) {
            e = Error( ErrorType::E_NumericOverflow, "I really can't do it. I'm gonna overflow with these:", { stack.fmt( op1 ), stack.fmt( op2 ) } );
            return std::string();
        }
    }
    else {
        if ( overflow( op1.value( ).value<float>(), op2.value( ).value<float>(), OPER_MUL ) ) {
            e = Error( ErrorType::E_NumericOverflow, "I really can't do it. I'm gonna overflow with these:", { stack.fmt( op1 ), stack.fmt( op2 ) } );
            return std::string();
        }
    }
    stack.push( op1 * op2 );
    return stack.fmt( stack.top( ) );
}


static std::string
arithm_div( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    if ( op1.value( ).can_treat_as<int>() && op2.value( ).can_treat_as<int>() )
    {
        if ( op2.value().value<int>() == 0 ) {
            e = Error( ErrorType::E_DivideByZero, "Divide by 0? How do you do that?", { stack.fmt( op1 ), stack.fmt( op2 ) } );
            return std::string();
        }
        if ( overflow( op1.value().value<int>(), op2.value().value<int>(), OPER_DIV ) ) {
            e = Error( ErrorType::E_NumericOverflow, "I really can't do it. I'm gonna overflow with these:", { stack.fmt( op1 ), stack.fmt( op2 ) } );
            return std::string();
        }
    }
    else {
        if ( op2.value().value<float>() == 0.0f ) {
            e = Error( ErrorType::E_DivideByZero, "Divide by 0? How do you do that?", { stack.fmt( op1 ), stack.fmt( op2 ) } );
            return std::string();
        }
        if ( overflow( op1.value().value<float>(), op2.value().value<float>(), OPER_DIV ) ) {
            e = Error( ErrorType::E_NumericOverflow, "I really can't do it. I'm gonna overflow with these:", { stack.fmt( op1 ), stack.fmt( op2 ) } );
            return std::string();
        }
    }
    stack.push( op1 / op2 );
    return stack.fmt( stack.top( ) );
}


static std::string
arithm_mod( OpStack & stack, Error & e )
{
    auto op2 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    if ( !op1.value( ).can_treat_as<int>( ) ) {
        e = Error( ErrorType::E_InvalidNumericValue, "Where have all the integers gone?", { stack.fmt( op1 ) } );
        return std::string();
    }
    if ( !op2.value( ).can_treat_as<int>( ) ) {
        e = Error( ErrorType::E_InvalidNumericValue, "Where have all the integers gone?", { stack.fmt( op2 ) } );
        return std::string();
    }
    if ( op2.value().value<int>() == 0 ) {
        e = Error( ErrorType::E_DivideByZero, "Divide by 0? How do you do that?", { stack.fmt( op1 ), stack.fmt( op2 ) } );
        return std::string();
    }
    stack.push( op1 % op2 );
    return stack.fmt( stack.top( ) );
}

static std::string
arithm_inc( OpStack & stack, Error & e )
{
    auto op = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    if ( op.value( ).can_treat_as<int>() )
    {
        if ( overflow( op.value().value<int>(), 1, OPER_ADD ) ) {
            e = Error( ErrorType::E_NumericOverflow, "Are you sure? Adding 1 is dangerous. It can spill out.", { stack.fmt( op ) } );
            return std::string();
        }
    }
    else {
        if ( overflow( op.value().value<float>(), 1.0f, OPER_ADD ) ) {
            e = Error( ErrorType::E_NumericOverflow, "Are you sure? Adding 1 is dangerous. It can spill out.", { stack.fmt( op ) } );
            return std::string();
        }
    }
    stack.push( op + Operand::fromInt( 1 ) );
    return stack.fmt( stack.top( ) );
}

static std::string
arithm_dec( OpStack & stack, Error & e )
{
    auto op = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    if ( op.value( ).can_treat_as<int>() )
    {
        if ( overflow( op.value().value<int>(), -1, OPER_ADD ) ) {
            e = Error( ErrorType::E_NumericOverflow, "Are you sure? Substracting 1 is dangerous. You could be left with more than nothing.", { stack.fmt( op ) } );
            return std::string();
        }
    }
    else {
        if ( overflow( op.value().value<float>(), -1.0f, OPER_ADD ) ) {
            e = Error( ErrorType::E_NumericOverflow, "Are you sure? Substracting 1 is dangerous. You could be left with more than nothing.", { stack.fmt( op ) } );
            return std::string();
        }
    }
    stack.push( op + Operand::fromInt(-1) );
    return stack.fmt( stack.top( ) );
}
