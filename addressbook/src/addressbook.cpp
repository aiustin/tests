//
//  addressbook.cpp
//
//  Created by Iustin Amihaesei on 23/01/2020.
//  Copyright © 2020 Iustin Amihaesei. All rights reserved.
//

#include "addressbook.h"

#include <cassert>
#include <set>


AddressBook::AddressBook( std::initializer_list<AddressBookEntry> entries ) noexcept :
   _d( std::make_shared<Data>( ) )
{
   for( const auto & entry : entries )
   {
      assert( entry.isValid( ) );
      if ( ! entry.isValid( ) ) {
         continue;
      }
      insertInto<ABEProperty::ABEP_FirstName, ABEProperty::ABEP_LastName>( _d->_byFirstName, entry );
      insertInto<ABEProperty::ABEP_LastName, ABEProperty::ABEP_FirstName>( _d->_byLastName, entry );
   }
}




bool
AddressBook::addEntry( const AddressBookEntry & entry ) noexcept
{
   if ( ! entry.isValid( ) )
   {
      assert( false );
      return false;
   }
   
   insertInto<ABEProperty::ABEP_FirstName, ABEProperty::ABEP_LastName>( _d->_byFirstName, entry );
   insertInto<ABEProperty::ABEP_LastName, ABEProperty::ABEP_FirstName>( _d->_byLastName, entry );
   return true;
}


AddressBookEntry
AddressBook::removeFirstEntry( const AddressBookEntry & entry ) noexcept
{
   if ( ! entry.isValid( ) )
   {
      assert( false );
      return AddressBookEntry( AddressBookEntry::Data{ } );
   }
   
   auto old_entry = removeFirst<ABEProperty::ABEP_FirstName>( _d->_byFirstName, entry );
   if ( ! old_entry.isValid( ) ) {
      return old_entry;
   }
   auto second_old_entry = removeFirst<ABEProperty::ABEP_LastName>( _d->_byLastName, entry );
   assert( second_old_entry.isValid( ) );
   return old_entry;
}


AddressBookEntries
AddressBook::removeAllEntries( const AddressBookEntry & entry ) noexcept
{
   if ( ! entry.isValid( ) )
   {
      assert( false );
      return AddressBookEntries( );
   }
   
   auto old_entries = removeAll<ABEProperty::ABEP_FirstName>( _d->_byFirstName, entry );
   if ( old_entries.empty( ) ) {
      return old_entries;
   }
   auto second_old_entries = removeAll<ABEProperty::ABEP_LastName>( _d->_byLastName, entry );
   assert( second_old_entries.size( ) == old_entries.size( ) );
   return old_entries;
}


AddressBookEntries
AddressBook::imatch( const std::string & name ) const
{
   std::set<int> added_entries;
   AddressBookEntries entries;
   if ( name.empty( ) )
   {
      assert( false );
      return entries;
      // or don't assert and return everything
   }
   
   auto iter = std::lower_bound( _d->_byFirstName.begin( ), _d->_byFirstName.end( ), AddressBookEntry( AddressBookEntry::Data{ name } ), [] ( const AddressBookEntry & a, const AddressBookEntry & b ) -> bool {
      return a.compare<ABEProperty::ABEP_FirstName>( b ) < 0;
   } );
   for ( ; iter != _d->_byFirstName.end( ) && iter->imatch<ABEProperty::ABEP_FirstName>( name ); ++iter )
   {
      entries.push_back( *iter );
      added_entries.insert( iter->hash( ) );
   }
   iter = std::lower_bound( _d->_byLastName.begin( ), _d->_byLastName.end( ), AddressBookEntry( AddressBookEntry::Data{ std::string( ), name } ), [] ( const AddressBookEntry & a, const AddressBookEntry & b ) -> bool {
      return a.compare<ABEProperty::ABEP_LastName>( b ) < 0;
   } );
   for ( ; iter != _d->_byLastName.end( ) && iter->imatch<ABEProperty::ABEP_LastName>( name ); ++iter )
   {
      if ( added_entries.find( iter->hash( ) ) == added_entries.end( ) ) {
         entries.push_back( *iter );
      }
   }
   return entries;
}
