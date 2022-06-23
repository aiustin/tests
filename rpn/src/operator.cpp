//
// Created by Iustin Amihaesei on 18/10/2020.
//

#include "operator.h"

#include "operand.h"

#include <sstream>
#include <fstream>
#include <math.h>

const char * g_helpStr = R"(
   Arithmetic Operators

      +          Add
      -          Subtract
      *          Multiply
      /          Divide
      %          Modulus
      ++         Increment
      --         Decrement

    Bitwise Operators

      &          Bitwise AND
      |          Bitwise OR
      ^          Bitwise XOR
      ~          Bitwise NOT
      <<         Bitwise shift left
      >>         Bitwise shift right

    Boolean Operators

      !          Boolean NOT
      &&         Boolean AND
      ||         Boolean OR
      ^^         Boolean XOR
      ?:         Ternary operator ( e.g. @a 1 2 ?: )

    Comparison Operators

      <          Less than
      <=         Less than or equal to
      ==         Equal to
      !=         Not equal to
      >          Greater than
      >=         Greater than or equal to

    Trigonometric Functions

      acos       Arc Cosine
      asin       Arc Sine
      atan       Arc Tangent
      cos        Cosine
      cosh       Hyperbolic Cosine
      sin        Sine
      sinh       Hyperbolic Sine
      tanh       Hyperbolic tangent

    Numeric Utilities

      ceil       Ceiling
      floor      Floor
      round      Round
      ip         Integer part
      fp         Floating part
      sign       Push -1, 0, or 0 depending on the sign
      abs        Absolute value
      max        Max
      min        Min

    Display Modes

      stack      Toggles stack display from horizontal to vertical
      verb       Toggled verbose mode
      hex        Switch display mode to hexadecimal
      dec        Switch display mode to decimal (default)
      bin        Switch display mode to binary
      oct        Switch display mode to octal

    Constants

      e          Push e
      pi         Push Pi
      rand       Generate a random number

    Mathematic Functions

      exp        Exponentiation
      fact       Factorial
      sqrt       Square Root
      ln         Natural Logarithm
      log10      Base 10 Logarithm
      log2       Base 2 Logarithm
      pow        Raise a number to a power

    Stack Manipulation

      ps         Print the stack and variables
      cla        Clear the stack and variables
      clr        Clear the stack
      clv        Clear the variables
      pick       Pick the -n'th item from the stack
      depth      Push the current stack depth
      drop       Drops the top item from the stack
      dropn      Drops n items from the stack
      dup        Duplicates the top stack item
      dupn       Duplicates the top n stack items in order
      swap       Swap the top 2 stack items

    Macros and Variables

      `          Defines a raw expression to be evaluated later, e.g. '`1024 *`'
                 (experimental; like Trinity way before explosion)
      :=         Assigns a variable, a function or a macro,
                    e.g. '@x 1024 :=' ; '@a `1024 *` :=' ; '#kb `1024 *` :=' then use it like this: '10kb'
      =          Evaluates the last value on stack

    Other

      help       Print the help message
      quit       Quit the calculator
      file       Reads and evaluates content of a file ( e.g. "test.rpn" file )
)";


#include "operators/op_math_p.cpp"
#include "operators/op_arithm_p.cpp"
#include "operators/op_disp_p.cpp"
#include "operators/op_other_p.cpp"
#include "operators/op_bw_p.cpp"
#include "operators/op_bool_p.cpp"
#include "operators/op_comp_p.cpp"
#include "operators/op_var_p.cpp"
#include "operators/op_stack_p.cpp"
#include "operators/op_trig_p.cpp"
#include "operators/op_num_p.cpp"












/*static */ const std::unordered_map< std::string, std::pair<Operator::OperatorFn, size_t> > Operator::_supportedOperators( {
        // Arithmetic Operators
        { "+",      { arithm_plus, 2 } },
        { "-",      { arithm_minus, 2 } },
        { "*",      { arithm_mul, 2 } },
        { "/",      { arithm_div, 2 } },
        { "%",      { arithm_mod, 2 } },
        { "--",     { arithm_dec, 1 } },
        { "++",     { arithm_inc, 1 } },

        // Display Modes
        { "stack",  { disp_stack, 0 } }, // Toggles stack display from horizontal to vertical
        { "verb",   { disp_verb, 0 } },
        { "hex",    { disp_hex, 0 } },
        { "dec",    { disp_dec, 0 } },
        { "bin",    { disp_bin, 0 } },
        { "oct",    { disp_oct, 0 } },

        // Other
        { "help",   { other_help, 0 } },
        { "quit",   { other_quit, 0 } },
        { "file",   { other_file, 1 } },

        // Bitwise Operators
        { "&",      { bw_and, 2 } },
        { "|",      { bw_or, 2 } },
        { "^",      { bw_xor, 2 } },
        { "~",      { bw_not, 1 } },
        { "<<",     { bw_shl, 2 } },
        { ">>",     { bw_shr, 2 } },

        // Boolean Operators
        { "!",      { b_not, 1 } },
        { "&&",     { b_and, 2 } },
        { "||",     { b_or, 2 } },
        { "^^",     { b_xor, 2 } },
        { "?:",     { b_ternary, 3 } },

        // Comparison Operators
        { "<",      { comp_lt, 2 } },
        { "<=",     { comp_le, 2 } },
        { "==",     { comp_e, 2 } },
        { "!=",     { comp_ne, 2 } },
        { ">",      { comp_gt, 2 } },
        { ">=",     { comp_ge, 2 } },


        // Variables
        { ":=",     { var_assign, 2 } },
        { "=",      { var_eval, 2 } },

        // Stack Manipulation
        { "ps",     { stack_ps, 0 } },
        { "cla",    { stack_cla, 0 } },
        { "clr",    { stack_clr, 0 } },
        { "clv",    { stack_clv, 0 } },
        { "pick",   { stack_pick, 1 } }, // Pick the -n'th item from the stack
//        { "repeat",   { stack_clv, 0 } }, // Repeat an operation n times, e.g. '3 repeat +'
        { "depth",  { stack_depth, 0 } }, // Push the current stack depth
        { "drop",   { stack_drop, 0 } }, // Drops the top item from the stack
        { "dropn",  { stack_dropn, 1 } }, // Drops n items from the stack
        { "dup",    { stack_dup, 0 } }, // Duplicates the top stack item
        { "dupn",   { stack_dupn, 1 } }, // Duplicates the top n stack items in order
        { "swap",   { stack_swap, 0 } }, // Swap the top 2 stack items

        // Trigonometric Functions
        { "acos",   { trig_acos, 1 } },
        { "asin",   { trig_asin, 1 } },
        { "atan",   { trig_atan, 1 } },
        { "cos",    { trig_cos, 1 } },
        { "cosh",   { trig_cosh, 1 } },
        { "sin",    { trig_sin, 1 } },
        { "sinh",   { trig_sinh, 1 } },
        { "tanh",   { trig_tanh, 1 } },

        // Numeric Utilities
        { "ceil",   { num_ceil, 1 } }, // Ceiling
        { "floor",  { num_floor, 1 } }, // Floor
        { "round",  { num_round, 1 } }, // Round
        { "ip",     { num_ip, 1 } }, // Integer part
        { "fp",     { num_fp, 1 } }, // Floating part
        { "sign",   { num_sign, 1 } }, // Push -1, 0, or 0 depending on the sign
        { "abs",    { num_abs, 1 } }, // Absolute value
        { "max",    { num_max, 2 } }, // Max
        { "min",    { num_min, 2 } }, // Min

        // Mathematic Functions
        { "exp",    { math_exp, 1 } }, // Exponentiation
        { "fact",   { math_fact, 1 } }, // Factorial
        { "sqrt",   { math_sqrt, 1 } }, // Square Root
        { "ln",     { math_ln, 1 } }, // Natural Logarithm
        { "log10",  { math_log10, 1 } }, // Base 10 Logarithm
        { "log2",   { math_log2, 1 } }, // Base 2 Logarithm
        { "pow",    { math_pow, 2 } }, // Raise a number to a power
                                                                                                                            } );


/*static */ std::string
Operator::exec( OpStack & stack, const std::string & op, Error & err )
{
    assert( isOperator( op ) );
    if ( ! isOperator( op ) ) {
        return std::string( );
    }
    auto fn_data = _supportedOperators.at(op);
    err = Error();
    if ( stack.canPop( fn_data.second ) ) {
        return fn_data.first( stack, err );
    }
    err = Error(ErrorType::E_NotEnoughOperands, "Not enough operands.");
    return std::string();
}