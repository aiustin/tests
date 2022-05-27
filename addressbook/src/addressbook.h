//
//  addressbook.h
//
//  Created by Iustin Amihaesei on 23/01/2020.
//  Copyright © 2020 Iustin Amihaesei. All rights reserved.
//

#ifndef _ADDRESSBOOK_H_
#define _ADDRESSBOOK_H_

#include "addressbookentry.h"

#include <vector>

using AddressBookEntries = std::vector<AddressBookEntry>;

class AddressBook
{
public:
   AddressBook( ) noexcept : _d( std::make_shared<Data>( ) ) { }
   AddressBook( std::initializer_list<AddressBookEntry> entries ) noexcept;

   bool addEntry( const AddressBookEntry & entry ) noexcept;
   inline bool addEntry( const std::string & firstName, const std::string & lastName ) noexcept { return addEntry( AddressBookEntry( { firstName, lastName } ) ); }
   inline AddressBookEntry removeFirstEntry( const std::string & firstName, const std::string & lastName ) noexcept { return removeFirstEntry( AddressBookEntry( { firstName, lastName } ) ); }
   inline void removeAllEntries( const std::string & firstName, const std::string & lastName ) noexcept { removeAllEntries( AddressBookEntry( { firstName, lastName } ) ); }

   inline const AddressBookEntries & entriesByFirstName( ) const { return _d->_byFirstName; }
   inline AddressBookEntries entriesByFirstName( const std::string & name ) const { return entries<ABEProperty::ABEP_FirstName>( _d->_byFirstName, AddressBookEntry( AddressBookEntry::Data{ name } ) ); }
   inline const AddressBookEntries & entriesByLastName( ) const { return _d->_byLastName; }
   inline AddressBookEntries entriesByLastName( const std::string & name ) const { return entries<ABEProperty::ABEP_LastName>( _d->_byLastName, AddressBookEntry( AddressBookEntry::Data{ std::string( ), name } ) ); }
   AddressBookEntries imatch( const std::string & name ) const;
   
private:
   AddressBookEntry removeFirstEntry( const AddressBookEntry & entry ) noexcept;
   AddressBookEntries removeAllEntries( const AddressBookEntry & entry ) noexcept;
   
   template<ABEProperty P, ABEProperty PS> void insertInto( AddressBookEntries & c, const AddressBookEntry & entry ) noexcept;
   template<ABEProperty P> AddressBookEntry removeFirst( AddressBookEntries & c, const AddressBookEntry & entry ) noexcept;
   template<ABEProperty P> AddressBookEntries removeAll( AddressBookEntries & c, const AddressBookEntry & entry ) noexcept;
   template<ABEProperty P> AddressBookEntries entries( AddressBookEntries & c, const AddressBookEntry & entry ) const noexcept;
   
private:
   struct Data
   {
      AddressBookEntries _byFirstName;
      AddressBookEntries _byLastName;
   };
   
   std::shared_ptr<Data> _d;
};



template<ABEProperty P, ABEProperty PS> void
AddressBook::insertInto( AddressBookEntries & c, const AddressBookEntry & entry ) noexcept
{
   auto iter = std::lower_bound( c.begin( ), c.end( ), entry, [] ( const AddressBookEntry & a, const AddressBookEntry & b ) -> bool { return a.compare<P>( b ) < 0; } );
   for ( ; iter != c.end( ) && iter->template compare<P>( entry ) == 0 && iter->template compare<PS>( entry ) < 0; ++iter );
   c.insert( iter, entry );
}


template<ABEProperty P> AddressBookEntry
AddressBook::removeFirst( AddressBookEntries & c, const AddressBookEntry & entry ) noexcept
{
   auto iter = std::lower_bound( c.begin( ), c.end( ), entry, [] ( const AddressBookEntry & a, const AddressBookEntry & b ) -> bool { return a.compare<P>( b ) < 0; } );
   for ( ; iter != c.end( ) && ! entry.compare<P>( *iter ); ++iter )
   {
      if ( entry.property<ABEProperty::ABEP_FirstName>( ) == iter->template property<ABEProperty::ABEP_FirstName>( ) &&
           entry.property<ABEProperty::ABEP_LastName>( ) == iter->template property<ABEProperty::ABEP_LastName>( )
          )
      {
         auto old_entry = *iter;
         c.erase( iter );
         return old_entry;
      }
   }
   return AddressBookEntry( AddressBookEntry::Data{ } );
}


template<ABEProperty P> AddressBookEntries
AddressBook::removeAll( AddressBookEntries & c, const AddressBookEntry & entry ) noexcept
{
   AddressBookEntries old_entries;
   auto iter = std::lower_bound( c.begin( ), c.end( ), entry, [] ( const AddressBookEntry & a, const AddressBookEntry & b ) -> bool { return a.compare<P>( b ) < 0; } );
   for ( ; iter != c.end( ) && ! entry.compare<P>( *iter ); )
   {
      if ( entry.property<ABEProperty::ABEP_FirstName>( ) == iter->template property<ABEProperty::ABEP_FirstName>( ) &&
          entry.property<ABEProperty::ABEP_LastName>( ) == iter->template property<ABEProperty::ABEP_LastName>( )
          )
      {
         old_entries.push_back( *iter );
         iter = c.erase( iter );
      }
      else {
         ++iter;
      }
   }
   return old_entries;
}


template<ABEProperty P> AddressBookEntries
AddressBook::entries( AddressBookEntries & c, const AddressBookEntry & entry ) const noexcept
{
   AddressBookEntries old_entries;
   auto iter = std::lower_bound( c.begin( ), c.end( ), entry, [] ( const AddressBookEntry & a, const AddressBookEntry & b ) -> bool { return a.compare<P>( b ) < 0; } );
   for ( ; iter != c.end( ) && ! entry.compare<P>( *iter ); ++iter )
   {
      if ( entry.property<P>( ) == iter->template property<P>( ) ) {
         old_entries.push_back( *iter );
      }
   }
   return old_entries;
}



#endif /* _ADDRESSBOOK_H_ */
