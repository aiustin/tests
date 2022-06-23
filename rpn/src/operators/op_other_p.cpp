//
// Created by Iustin Amihaesei on 22/10/2020.
//

// Other

static std::string
other_help( OpStack & stack, Error & e )
{
    return g_helpStr;
}

static std::string
other_quit( OpStack & stack, Error & e )
{
    e = Error( ErrorType::E_Quit, std::string( ) );
    return std::string( );
}

#include <iostream>
static std::string
other_file( OpStack & stack, Error & e )
{
    auto op = stack.popString( e );
    if ( ! e.success( ) ) {
        return std::string( );
    }
    std::ifstream in_file( op.value( ).asString( ) );
    if ( ! in_file.is_open( ) ) {
        e = Error( ErrorType::E_InvalidFile, "This file really wants to stay closed.", {  op.value( ).asString( ) } );
        return std::string( );
    }
    std::string expr( ( std::istreambuf_iterator<char>( in_file ) ), std::istreambuf_iterator<char>( ) );
    in_file.close();
    std::string output;
    if ( stack.eval( expr, e, output ) ) {
        if ( stack.size( ) ) {
            output = stack.fmt( stack.top( ) );
        }
    }
    return output;
}
