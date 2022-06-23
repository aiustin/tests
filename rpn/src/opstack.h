//
// Created by Iustin Amihaesei on 18/10/2020.
//

#ifndef RPN_OPSTACK_H
#define RPN_OPSTACK_H

#include "operand.h"
#include "error.h"

#include <stack>
#include <unordered_map>
#include <sstream>

class OpStack
{
public:
    std::string push( const std::string & value, Error * err = nullptr );
    void push( const Operand & value );
    Operand top( ) const;

    Operand popRaw( ) { return pop( ); }
    Operand pop( Error & e );
    Operand popNumeric( Error & e );
    Operand popString( Error & e );
    bool canPop( size_t count ) { return count <= size( ); }
    bool eval( const std::string & expr, Error & e, std::string & output  );
    bool eval( const std::vector<std::string> & expr, Error & e, std::string & output );
    bool eval( Operand & op, Error & e, std::string * output = nullptr );
    void assign( Operand op, OperandValue value );

    int size( ) const { return _stack.size( ); }
    bool isEmpty( ) const { return _stack.empty( ); }

    void setBase( unsigned char b ) { _state.base = b; }
    unsigned char base( ) { return _state.base; }

    void toggleVerbose( ) { _state.verbose = ! _state.verbose; }
    bool isVerboseOn( ) const { return _state.verbose; }
    void toggleStackDisplay( ) { _state.displayVertically = ! _state.displayVertically; }

    void clr( ) { _stack.clear( ); }
    void clv( ) { _vars.clear( ); _macros.clear( ); }
    void cla( ) { clr(); clv(); }

    std::string fmt( const Operand & op );
    std::string fmt( );

private:
    Operand pop( );

private:
    static std::string fmt( int value, int base );
    template<typename T> std::string echo( T v );
    bool evalExpression( Operand & op, Error & e, std::string * output = nullptr );
    std::string preprocess( const std::string & value );

private:
    struct State
    {
        bool verbose = false;
        unsigned char base = 10;
        bool displayVertically = true;
    };

    std::vector<Operand> _stack;
    State _state;
    std::unordered_map<std::string, Operand> _vars;
    std::unordered_map<std::string, Operand> _macros;
    unsigned long long _anonymousCount = 0;

    friend std::ostream& operator<<( std::ostream & os, const OpStack & stack );
};


template<typename T> std::string
OpStack::echo( T v )
{
    std::ostringstream oss;
    if( _state.verbose ) {
        oss << v;
    }
    return oss.str();
}


#endif //RPN_OPSTACK_H
