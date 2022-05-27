//
//  addressbookentry.cpp
//
//  Created by Iustin Amihaesei on 23/01/2020.
//  Copyright © 2020 Iustin Amihaesei. All rights reserved.
//

#include "addressbookentry.h"

#include <cassert>
#include <algorithm>
#include <cctype>


/*static */ int
AddressBookEntry::stricmp( const char * a, const char *ae, const char * b, const char * be ) noexcept
{
   assert( a );
   assert( b );
   
   const char * min_e = std::min( a + (be - b), ae );
   for ( ; a < min_e ; ++a, ++b )
   {
      int diff = std::tolower( *a ) - std::tolower( *b );
      if ( diff ) {
         return diff;
      }
   }
   return a == ae ? ( b == be ? 0 : -1 ) : 1;
}


/*static */ bool
AddressBookEntry::strimatch( const char * a, const char *ae, const char * b, const char * be ) noexcept
{
   assert( a );
   assert( b );
   
   if ( be - b > ae - a ) {
      return false;
   }
   for ( ; b < be && std::tolower( *a ) == std::tolower( *b ) ; ++a, ++b );
   return b >= be;
}


int
AddressBookEntry::compare( const AddressBookEntry & other, PropertyType<ABEProperty::ABEP_FirstName> ) const noexcept
{
   if ( _d == other._d ) {
      return 0;
   }
   const auto & a = _d->_firstName;
   const auto & b = other._d->_firstName;
   return stricmp( a.data( ), a.data( ) + a.size( ), b.data( ), b.data( ) +b.size( ) );
}


int
AddressBookEntry::compare( const AddressBookEntry & other, PropertyType<ABEProperty::ABEP_LastName> ) const noexcept
{
   if ( _d == other._d ) {
      return 0;
   }
   const auto & a = _d->_lastName;
   const auto & b = other._d->_lastName;
   return stricmp( a.data( ), a.data( ) + a.size( ), b.data( ), b.data( ) +b.size( ) );
}




