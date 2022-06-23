//
// Created by Iustin Amihaesei on 22/10/2020.
//


static std::string
stack_ps( OpStack & stack, Error & e )
{
    return stack.fmt( );
}

static std::string
stack_clr( OpStack & stack, Error & e )
{
    stack.clr( );
    return std::string( );
}

static std::string
stack_cla( OpStack & stack, Error & e )
{
    stack.cla( );
    return std::string( );
}

static std::string
stack_clv( OpStack & stack, Error & e )
{
    stack.clv( );
    return std::string( );
}

static std::string
stack_pick( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    if ( ! op1.value( ).isInt( ) || op1.value( ).value<int>( ) < 0 || op1.value( ).value<int>( ) > stack.size( ) - 1 ) {
        e = Error( ErrorType::E_InvalidNumericValue, "I'm a bit picky on this:", { stack.fmt( op1 ) } );
        return std::string();
    }

    // TODO: optimize this a bit by implementing it in the stack.
    if ( op1.value( ).value<int>( ) > 1 ) {
        std::vector<Operand> ops;
        for( auto i = 0; i < op1.value( ).value<int>( ); ++i ) {
            ops.push_back( stack.popRaw( ) );
        }
        for( auto i = 0; i < ops.size( ) - 1; ++i ) {
            stack.push( ops[ i ] );
        }
        stack.push( ops[ ops.size( ) - 1 ] );
    }

    return stack.fmt( stack.top( ) );
}

static std::string
stack_depth( OpStack & stack, Error & e )
{
    const auto depth = stack.size( );
    stack.push( Operand::fromInt( depth ) );

    return stack.fmt( stack.top( ) );
}

static std::string
stack_drop( OpStack & stack, Error & e )
{
    return stack.fmt( stack.popRaw( ) );
}

static std::string
stack_dropn( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    if ( ! op1.value( ).isInt( ) || op1.value( ).value<int>( ) < 0 ) {
        e = Error( ErrorType::E_InvalidOperand, "I'm not gonna drop this. It'll definitely break.", { stack.fmt( op1 ) } );
        return std::string();
    }
    std::string s;
    for( auto i = 0; i < op1.value( ).value<int>( ) && !stack.isEmpty( ); ++i ) {
        s += stack.fmt( stack.popRaw( ) ) + " ";
    }

    return s;
}

static std::string
stack_dup( OpStack & stack, Error & e )
{
    if ( stack.isEmpty( ) ) {
        e = Error( ErrorType::E_InvalidOperand, "Ohhh.. My stack is so empty. Please fill it first.", { } );
        return std::string();
    }
    auto op1 = stack.popRaw( );
    stack.push( op1 );
    stack.push( op1 );

    return stack.fmt( stack.top( ) );
}

static std::string
stack_dupn( OpStack & stack, Error & e )
{
    auto op1 = stack.popNumeric( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    if ( ! op1.value( ).isInt( ) || op1.value( ).value<int>( ) < 0 ) {
        e = Error( ErrorType::E_InvalidOperand, "I can't clone myself so many times...", { stack.fmt( op1 ) } );
        return std::string();
    }
    std::vector<Operand> ops;
    for(auto i = 0; i < op1.value( ).value<int>( ); ++i ) {
        ops.push_back( stack.popRaw( ) );
    }

    std::string s;
    for( int i = 0; i < 2; ++i ) {
        for( const auto & op : ops ) {
            stack.push( op );
            if ( i ) {
                s += stack.fmt( op ) + " ";
            }
        }
    }

    return s;
}

static std::string
stack_swap( OpStack & stack, Error & e )
{
    if ( stack.size( ) < 2 ) {
        e = Error( ErrorType::E_InvalidOperand, "Not enough stuff to work on. Bring me more.", { } );
        return std::string();
    }
    auto op1 = stack.popRaw( );
    auto op2 = stack.popRaw( );
    stack.push( op1 );
    stack.push( op2 );

    return stack.fmt( op1 ) + " " + stack.fmt( op2 );
}
