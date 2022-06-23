#include <iostream>

#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <sys/stat.h>

#include "opstack.h"

static constexpr char const * g_optionsHelpStr = R"(
USAGE:
    rpn [option] ... [-c cmd | file | -]

    Options and arguments:
        -E      : ignore ~/.rpnrc
        -h      : print this help message and exit
        -q      : quiet. Don't print any output (ignored in interactive mode)
        -v      : verbose (traces every push statement)

        [-e expr | file | -]: any of these will terminate the options list
        -e expr : expression passed in as string
        file    : expression read from file
        -       : expression read from stdin (default; interactive mode if a tty)

RC FILE (Unix only ; for now)
      rpn will execute the contents of ~/.rpnrc at startup if it exists and none of [-c cmd | file | -] was used.

EXAMPLES

)";

static constexpr char const * g_rcfile = ".rpnrc";

struct Environment
{
    const char * prompt( ) const
    {
        return "> ";
    }

    bool interactive = false;
    bool quiet = false;
    bool ignoreRc = false;
    bool verbose = false;
    std::string expr;
    std::string file;
};



#if 0
#define KEY_LEFT 75
#define KEY_RIGHT 77
#endif

bool processOptions( int argc, char * argv[ ], Environment & env );
int evalStdin( const Environment & env, OpStack & stack, Error & e, std::string & output );

int
main( int argc, char * argv[ ] )
{
    OpStack stack;
    std::string output;
    Error e;
    bool ret = 0;
    Environment env;
    env.interactive = isatty( fileno( stdin ) );

    if ( ! processOptions( argc, argv, env ) ) {
        return 0;
    }


    bool stop = false;

    if ( ! env.ignoreRc ) {
        std::string file_path = std::getenv("HOME");
        file_path = file_path.empty( ) ? "./" : file_path + "/";
        file_path += g_rcfile;
        struct stat buffer;
        if ( stat( file_path.c_str( ), &buffer) == 0 ) {
            stack.eval( std::string( "\"" ) + file_path + "\" file", e, output );
            stop = true;
        }
    }

    if ( ! stop && ( ! env.file.empty( ) || ! env.expr.empty( ) ) )
    {
        if ( ! env.file.empty( ) ) {
            stack.eval( std::string( "\"" ) + env.file + "\" file", e, output );
        }
        else {
            stack.eval( env.expr, e, output );
        }
        stop = true;
    }

    if ( ! stop ) {
        stop = true;
        if ( env.interactive ) {
            env.quiet = false;
        }
        ret = evalStdin( env, stack, e, output );
    }

    if ( e.success( ) && ! env.quiet && ! output.empty( ) ) {
        std::cout << output << std::endl;
    }
    if ( ! e.success( ) ) {
        std::cerr << "[Error] - " << e.message << " ";
        for( const auto & p : e.params ) {
            std::cerr << p << " ";
        }
        std::cerr << std::endl;
        ret = 1;
    }

    return ret;
}


bool
processOptions( int argc, char * argv[ ], Environment & env ) {
    if (argc < 1) {
        return true;
    }

    for (auto i = 1; i < argc; ++i) {
        std::string option = argv[i];
        assert(!option.empty());
        // TODO: this is quite messy design-wise but will do for this simple set of options
        if (option[0] == '-') {
            if (option.size() == 1) {
                env.ignoreRc = true;
                return true;
            }
            char option_c = option[1];
            if (option.size() > 2 ||
                (option_c != 'E' && option_c != 'h' && option_c != 'q' && option_c != 'v' && option_c != 'e')) {
                std::cerr << "Invalid option: " << option << std::endl;
            }
            if (option_c == 'h') {
                std::cout << g_optionsHelpStr << std::endl;
                return false;
            }
            if (option_c == 'E') {
                env.ignoreRc = true;
            } else if (option_c == 'q') {
                env.quiet = true;
            } else if (option_c == 'v') {
                env.verbose = true;
            } else if (option_c == 'e') {
                env.interactive = false;
                env.ignoreRc = true;
                env.file.clear( );
                env.expr.clear();
                for (auto j = i + 1; j < argc; ++j) {
                    env.expr += std::string(argv[j]) + " ";
                }
                return true;
            }
        } else {
            env.interactive = false;
            env.ignoreRc = true;
            env.expr.clear( );
            env.file = argv[i];
            return true;
        }
    }
    return true;
}

int
evalStdin( const Environment & env, OpStack & stack, Error & e, std::string & output )
{
    int ret = 0;
    while( ! e.quit( ) && ! std::cin.eof( ) )
    {
        if ( env.interactive ) {
            std::cout << env.prompt( );
        }
        std::string str_expr;
#if 0
        //TODO: add supoport for recalling previous lines and edit them.
        // Maybe see ncurses.
        char c;
        while (std::cin.get( c ) )
        {
            if ( c == '\n') {
                break;
            }
            if ( c == KEY_LEFT ) {

            }
            str_expr += c;
        }
#else
        std::getline(std::cin, str_expr);
#endif
        if ( std::cin.bad( ) )
        {
            // TODO: Print a more detailed error if possible
            std::cout << "An error occured while reading standard input." << std::endl;
            return 1;
        }
        stack.eval( str_expr, e, output );

        if ( e.success( ) && ! env.quiet && ! output.empty( ) ) {
            std::cout << output << std::endl;
        }
        if ( ! e.success( ) ) {
            std::cerr << "[Error] - " << e.message << " ";
            for( const auto & p : e.params ) {
                std::cerr << p << " ";
            }
            std::cerr << std::endl;
        }

        if ( ! env.interactive ) {
            break;
        }
    }
    return env.interactive ? 0 : ret;
}

