//
//  main.cpp
//
//  Created by Iustin Amihaesei on 23/01/2020.
//  Copyright © 2020 Iustin Amihaesei. All rights reserved.
//

#include <iostream>
#include <vector>
#include <chrono>

#include "addressbook.h"

int main(int argc, const char * argv[])
{

#if 1
   AddressBook address_book_speed;
   auto start = std::chrono::system_clock::now();
   for( auto i = 0; i < 100000; ++i  )
   {
      address_book_speed.addEntry( AddressBookEntry( AddressBookEntry::Data{ "abc", "xyz", std::to_string( i ) } ) );
   }
   auto end = std::chrono::system_clock::now();
   auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
   std::cout << elapsed.count() << std::endl;
#endif
   

   AddressBook address_book {
      { { "Dan", "Abc", "0123" } },
      { { "daniel", "abc", "0123" } },
      { { "George", "daNt", "0123" } },
      { { "Andrew", "Ksixw", "0123" } },
      { { "Xero", "Hero", "0123" } },
      { { "xero", "hero", "0123" } },
      { { "Xero", "Hero", "456" } },
   };
   
   address_book.addEntry( AddressBookEntry( AddressBookEntry::Data{ "abc", "xyz" } ) );
   address_book.addEntry( AddressBookEntry( AddressBookEntry::Data{ "xyz", "ABC" } ) );
   address_book.removeAllEntries( "Xero", "Hero" );
   address_book.addEntry( AddressBookEntry( AddressBookEntry::Data{ "daNut", "Danil" } ) );

   std::cout << "***** All entries by last name ****" << std::endl;
   const auto all_by_last_name = address_book.entriesByLastName( );
   for( const auto & entry : all_by_last_name )
   {
      std::cout << entry.property<ABEProperty::ABEP_LastName>( ) <<  "\t"  << entry.property<ABEProperty::ABEP_FirstName>( ) << "\t" <<entry.property<ABEProperty::ABEP_PhoneNumber>( ) << std::endl;
   }
   std::cout << "***** 'abc' entries by last name ****" << std::endl;
   const auto ab_by_last_name = address_book.entriesByLastName( "abc" );
   for( const auto & entry : ab_by_last_name )
   {
      std::cout << entry.property<ABEProperty::ABEP_LastName>( ) <<  "\t" << entry.property<ABEProperty::ABEP_FirstName>( ) << "\t" <<entry.property<ABEProperty::ABEP_PhoneNumber>( ) << std::endl;
   }

   std::cout << "***** All entries by first name ****" << std::endl;
   const auto all_by_first_name = address_book.entriesByFirstName( );
   for( const auto & entry : all_by_first_name )
   {
      std::cout << entry.property<ABEProperty::ABEP_FirstName>( ) << "\t" << entry.property<ABEProperty::ABEP_LastName>( ) <<  "\t" <<entry.property<ABEProperty::ABEP_PhoneNumber>( ) << std::endl;
   }

   std::cout << "***** 'abc' entries by first name ****" << std::endl;
   const auto ab_by_first_name = address_book.entriesByFirstName( "abc" );
   for( const auto & entry : ab_by_first_name )
   {
      std::cout << entry.property<ABEProperty::ABEP_FirstName>( ) << "\t" << entry.property<ABEProperty::ABEP_LastName>( ) <<  "\t" <<entry.property<ABEProperty::ABEP_PhoneNumber>( ) << std::endl;
   }
   
   std::cout << "***** 'Dan' entries by imatch ****" << std::endl;
   const auto ab_by_imatch = address_book.imatch( "Dan" );
   for( const auto & entry : ab_by_imatch )
   {
      std::cout << entry.property<ABEProperty::ABEP_FirstName>( ) << "\t" << entry.property<ABEProperty::ABEP_LastName>( ) <<  "\t" <<entry.property<ABEProperty::ABEP_PhoneNumber>( ) << std::endl;
   }

   return 0;
}
