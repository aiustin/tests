//
// Created by Iustin Amihaesei on 18/10/2020.
//

#include "operand.h"


std::ostream&
operator<<( std::ostream & os, const Operand & op )
{
    std::ios_base::fmtflags ff = os.flags( );

    if ( op.isVar( ) ) {
        if ( ff & os.skipws ) {
            os << op._var;
        }
        else {
            os << op._var << " = " << op._value;
        }
    }
    else {
        os << op._value;
    }
    return os;
}