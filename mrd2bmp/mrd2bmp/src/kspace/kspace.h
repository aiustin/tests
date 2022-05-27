//
//  kspace.h
//  mrd2bmp
//

#ifndef _KSPACE_H_
#define _KSPACE_H_

#include "datatypedefs.h"

#include <array>
#include <vector>
#include <complex>
#include <cassert>


/*********************************************************************
 *
 * KSpace
 * - defines a kspace of some SampleDataDimensions and loads it with SampleData
 * - kspace can be used to retrieve the result
 *
 *********************************************************************/


class MRDFile;

class KSpace
{
public:
   static_assert( std::is_trivially_copyable<SampleDataType>::value, "SampleData is not trivially copyable." );
   
private:
   KSpace( SampleDataType sdt, const SampleDataDimensions & dimensions, SampleData && data );
public:
   KSpace( SampleDataType sdt = SampleDataType::SDT_ComplexFloat, const SampleDataDimensions & dimensions = SampleDataDimensions{ 0, 0, 0, 0, 0, 0 } );
   KSpace( SampleDataType sdt, int dimensionX, int dimensionY ) : KSpace( sdt, SampleDataDimensions { dimensionX, dimensionY, 1, 1, 1, 1 } ) { }
   ~KSpace( ) =default;
   KSpace( const KSpace& ) =default;
   KSpace( KSpace&& ) =default;
   KSpace& operator=( const KSpace& ) =default;
   KSpace& operator=( KSpace&& ) =default;
   
   static KSpace fromFile( const std::string & filePath );
   static KSpace fromFile( MRDFile & mrdfile );
   
   SampleDataType sampleDataType( ) const { return _sampleDataType; }
   void setSampleDataType( SampleDataType sdt );
   
   SampleDataDimensions dimensions( ) const { return _dimensions; }
   int dimension( int i ) const { return _dimensions[ i ]; }
   bool isNull( ) const { return !( sampleSize( ) *  samplesCount( ) ); }
   int sampleSize( ) const { return _dimensions[ 0 ]; }
   int samplesPerView( ) const { return _dimensions[ 1 ]; }
   int samplesCount( ) const { return _dimensions[ 1 ] * _dimensions[ 2 ] * _dimensions[ 3 ] * _dimensions[ 4 ] * _dimensions[ 5 ]; }
   size_t size( ) const { return _samples.size( ); }

   SampleData sample( int sampleIdx ) const;
   SampleData sampleForView( int viewIdx, int sampleIdx ) const;
   SampleData samplesForView( int viewIdx ) const;
   
   void setSample( SampleData && data );
   void setSample( const SampleData & data );
   
private:
   SampleDataType _sampleDataType = SampleDataType::SDT_ComplexFloat;
   SampleDataDimensions _dimensions;
   SampleData _samples;
};


#endif /* _KSPACE_H_ */
