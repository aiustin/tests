//
// Created by Iustin Amihaesei on 22/10/2020.
//


// Display Modes


static std::string
disp_stack( OpStack & stack, Error & e )
{
    stack.toggleStackDisplay( );
    return std::string( );
}

static std::string
disp_verb( OpStack & stack, Error & e )
{
    stack.toggleVerbose();
    return std::string( );
}

static std::string
disp_hex( OpStack & stack, Error & e )
{
    stack.setBase( 16 );
    return std::string( );
}

static std::string
disp_dec( OpStack & stack, Error & e )
{
    stack.setBase( 10 );
    return std::string( );
}

static std::string
disp_bin( OpStack & stack, Error & e )
{
    stack.setBase( 2 );
    return std::string( );
}

static std::string
disp_oct( OpStack & stack, Error & e )
{
    stack.setBase( 8 );
    return std::string( );
}
