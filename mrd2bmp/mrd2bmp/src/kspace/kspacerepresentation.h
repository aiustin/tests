//
//  kspacerepresentation.h
//  mrd2bmp
//

#ifndef _KSPACEREPRESENTATION_H_
#define _KSPACEREPRESENTATION_H_

#include "kspace.h"

/*********************************************************************
 *
 * KSpaceRepresentation
 * - allow generating a representation in Magnitude or Phase of a KSpace
 * - the representation can be written into a BMP
 * - added support only for SDT_ComplexFloat
 *
 *********************************************************************/


class BMP;

class KSpaceRepresentation
{
public:
   enum Type
   {
      KSR_Magnitude  = 0,
      KSR_Phase      = 1
   };
public:
   KSpaceRepresentation( ) { }
   KSpaceRepresentation( const KSpace & kspace ) : _kspace( kspace ) { assert( kspace.sampleDataType( ) == SampleDataType::SDT_ComplexFloat ); }
   KSpaceRepresentation( KSpace && kspace ) : _kspace( std::move( kspace ) ) { assert( kspace.sampleDataType( ) == SampleDataType::SDT_ComplexFloat ); }
   
   const KSpace & kspace( ) const { return _kspace; }
   
   void generate( BMP & bmp, Type t );
   
private:
   // These are defined as templates because I also tested with support for double
   template<typename T> void normalize( std::vector<T> & data );
   template<typename T> std::vector<unsigned char> dataToColour( std::vector<T> & data );
   
   void generate( BMP & bmp, const std::vector<unsigned char> & data );

private:
   KSpace _kspace;
   std::vector<float> _representations[ 2 ];
};


template<typename T> void
KSpaceRepresentation::normalize( std::vector<T> & data )
{
   if ( data.empty( ) ) {
      return;
   }
   const auto minmax = std::minmax_element( data.begin( ), data.end( ) );
   const auto min = std::abs( *minmax.first );
   const auto max = min + std::abs( *minmax.second );
   std::transform( data.begin(), data.end(), data.begin( ), [ min, max ]( T v ) -> T { return ( min + v ) / max; } );
}


template<typename T> std::vector<unsigned char>
KSpaceRepresentation::dataToColour( std::vector<T> & data )
{
   std::vector<unsigned char> colour;
   std::transform( data.begin(), data.end(), std::back_inserter( colour ), [ ]( T v ) -> unsigned char { return (unsigned char) std::round( v * 255 ); } );
   return colour;
}


#endif /* _KSPACEREPRESENTATION_H_ */
