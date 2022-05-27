//
//  addressbookentry.h
//
//  Created by Iustin Amihaesei on 23/01/2020.
//  Copyright © 2020 Iustin Amihaesei. All rights reserved.
//

#ifndef _ADDRESSBOOKENTRY_H_
#define _ADDRESSBOOKENTRY_H_


#include <string>

enum class ABEProperty
{
   ABEP_FirstName,
   ABEP_LastName,
   ABEP_PhoneNumber,
   ABEP_TestError
};

template<ABEProperty p> struct typeOfABEP;
template<> struct typeOfABEP<ABEProperty::ABEP_FirstName>                   { using type = std::string; };
template<> struct typeOfABEP<ABEProperty::ABEP_LastName>                    { using type = std::string; };
template<> struct typeOfABEP<ABEProperty::ABEP_PhoneNumber>                 { using type = std::string; };



class AddressBookEntry
{
public:
   struct Data
   {
      inline bool isValid( ) const noexcept { return ! ( _firstName.empty( ) && _lastName.empty( ) ); }
      
      typeOfABEP<ABEProperty::ABEP_FirstName>::type _firstName;
      typeOfABEP<ABEProperty::ABEP_LastName>::type _lastName;
      typeOfABEP<ABEProperty::ABEP_PhoneNumber>::type _phoneNumber;
   };
   template<ABEProperty P, typename PT = typeOfABEP<P>> struct PropertyType {};
   
public:
   inline AddressBookEntry( ) noexcept = delete;
   inline AddressBookEntry( const Data & d ) noexcept : _d( std::make_shared<Data>( d ) ) { }
   
   inline AddressBookEntry( const AddressBookEntry & ) noexcept = default;
   inline AddressBookEntry( AddressBookEntry && ) noexcept  = default;
   inline AddressBookEntry & operator=( const AddressBookEntry & ) noexcept = default;
   inline AddressBookEntry & operator=( AddressBookEntry && ) noexcept = default;
   
   inline bool isValid( ) const noexcept { return _d->isValid( ); }
   
   template<ABEProperty P> auto property( ) const noexcept { return property( PropertyType<P>( ) ); }
   template<ABEProperty P> int compare( const AddressBookEntry & other ) const noexcept { return compare( other, PropertyType<P>( ) ); }
   template<ABEProperty P> bool imatch( const std::string & substr ) const noexcept { return imatch( substr, PropertyType<P>( ) ); }
   
   std::intptr_t hash( ) const noexcept { return reinterpret_cast<std::intptr_t>( _d.get( ) ); }
   
private:
   static int stricmp( const char * a, const char *ae, const char * b, const char * be ) noexcept;
   static bool strimatch( const char * a, const char *ae, const char * b, const char * be ) noexcept;

   inline auto property( PropertyType<ABEProperty::ABEP_FirstName> ) const noexcept { return _d->_firstName; }
   inline auto property( PropertyType<ABEProperty::ABEP_LastName> ) const noexcept { return _d->_lastName; }
   inline auto property( PropertyType<ABEProperty::ABEP_PhoneNumber> ) const noexcept { return _d->_phoneNumber; }
   
   int compare( const AddressBookEntry & other, PropertyType<ABEProperty::ABEP_FirstName> ) const noexcept;
   int compare( const AddressBookEntry & other, PropertyType<ABEProperty::ABEP_LastName> ) const noexcept;
   
   bool imatch( const std::string & substr, PropertyType<ABEProperty::ABEP_FirstName> ) const noexcept { return strimatch( _d->_firstName.data( ), _d->_firstName.data( ) + _d->_firstName.size( ), substr.data( ), substr.data( ) + substr.size( ) ); }
   bool imatch( const std::string & substr, PropertyType<ABEProperty::ABEP_LastName> ) const noexcept { return strimatch( _d->_lastName.data( ), _d->_lastName.data( ) + _d->_lastName.size( ), substr.data( ), substr.data( ) + substr.size( ) ); }


private:
   std::shared_ptr<Data> _d;
};



#endif /* _ADDRESSBOOKENTRY_H_ */
