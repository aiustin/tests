//
//  kspace.cpp
//  mrd2bmp
//


#include "kspace.h"

#include "mrdfile.h"

/*static */ KSpace
KSpace::fromFile( const std::string & filePath )
{
   MRDFile mrd_file( filePath );
   return fromFile( mrd_file );
}


/*static */ KSpace
KSpace::fromFile( MRDFile & mrdfile )
{
   return ! mrdfile.load( ) ? KSpace( ) : KSpace( mrdfile.sampleSataType( ), mrdfile.dimensions( ), mrdfile.data( ) );
}


KSpace::KSpace( SampleDataType sdt, const SampleDataDimensions & dimensions, SampleData && data ) :
   _sampleDataType( sdt )
,  _dimensions( dimensions )
,  _samples( std::move( data ) )
{
   assert( _samples.size( ) == samplesCount( ) * sampleSize( ) );
}


KSpace::KSpace( SampleDataType sdt /* = SampleDataType::SDT_ComplexFloat */, const SampleDataDimensions & dimensions /*= SampleDataDimensions{ 0, 0, 0, 0, 0, 0 } */ ) :
   _sampleDataType( sdt )
,  _dimensions( dimensions )
{
   if ( ! isNull( ) ) {
      _samples.resize( samplesCount( ) * sampleSize( ) );
   }
}


void
KSpace::setSampleDataType( SampleDataType sdt )
{
   if ( sdt == _sampleDataType ) {
      return;
   }
   _sampleDataType = sdt;
   std::fill( _samples.begin( ), _samples.end( ), SampleDataValue( ) );
}


SampleData
KSpace::sample( int sampleIdx ) const
{
   assert( sampleIdx >= 0 && static_cast<unsigned int>( sampleIdx ) * _dimensions[ 0 ] < _samples.size( ) );
   if ( sampleIdx < 0 || static_cast<unsigned int>( sampleIdx ) * _dimensions[ 0 ] >= _samples.size( ) ) {
      return SampleData( );
   }
   SampleData dest_sample( _dimensions[ 0 ] );
   // should be internaly optimised for TriviallyCopyable types to do a std::memmove
   std::copy( _samples.begin( ) +  sampleIdx * _dimensions[ 0 ],
              _samples.begin( ) +  ( sampleIdx + 1 ) * _dimensions[ 0 ], dest_sample.begin( ) );
   return dest_sample;
}


SampleData
KSpace::sampleForView( int viewIdx, int sampleIdx ) const
{
   assert( viewIdx >= 0 && viewIdx < _dimensions[ 1 ] );
   if ( viewIdx < 0 || viewIdx >= _dimensions[ 1] ) {
      return SampleData( );
   }
   SampleData dest_sample( _dimensions[ 0 ] );
   // should be internaly optimised for TriviallyCopyable types to do a std::memmove
   std::copy( _samples.begin( ) +  viewIdx * _dimensions[ 1 ] * _dimensions[ 0 ] + sampleIdx * _dimensions[ 0 ],
              _samples.begin( ) +  viewIdx * _dimensions[ 1 ] * _dimensions[ 0 ] + ( sampleIdx + 1 ) * _dimensions[ 0 ] , dest_sample.begin( ) );
   return dest_sample;
}

SampleData
KSpace::samplesForView( int viewIdx ) const
{
   assert( viewIdx >= 0 && viewIdx < _dimensions[ 1 ] );
   if ( viewIdx < 0 || viewIdx >= _dimensions[ 1] ) {
      return SampleData( );
   }
   SampleData dest_sample( _dimensions[ 0 ] * _dimensions[ 1 ] );
   // should be internaly optimised for TriviallyCopyable types to do a std::memmove
   std::copy( _samples.begin( ) +  viewIdx * _dimensions[ 1 ] * _dimensions[ 0 ],
              _samples.begin( ) +  ( viewIdx + 1 ) * _dimensions[ 1 ] * _dimensions[ 0 ], dest_sample.begin( ) );
   return dest_sample;
}


void
KSpace::setSample( SampleData && data )
{
   assert( sizeof( data ) == sizeof( _samples ) );
   _samples = std::move( data );
}


void
KSpace::setSample( const SampleData & data )
{
   assert( sizeof( data ) == sizeof( _samples ) );
   _samples = data;
}

