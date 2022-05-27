//
//  kspacetransform.cpp
//  mrd2bmp
//


#include "kspacetransform.h"

#include "fftw3.h"


/*********************************************************************
 *
 * KSpaceTransform implementation
 *
 *********************************************************************/


bool
KSpaceTransform::apply( )
{
   if ( _kspace.isNull( ) || isTransformed( ) ) {
      return false;
   }
   preprocess( _kspace );
   _transformed = transform( _kspace );
   if ( _transformed ) {
      postprocess( _kspace );
   }
   
   return _transformed;
}





/*********************************************************************
 *
 * KSpaceFFTTransform implementation
 *
 *********************************************************************/


KSpaceFFTTransform::~KSpaceFFTTransform( )
{
   if ( _in ) {
      fftw_free( _in );
   }
   if ( _out ) {
      fftw_free( _out );
   }
}


void
KSpaceFFTTransform::preprocess( KSpace & kspace )
{
   _n0 = kspace.dimension( 1 );
   _n1 = kspace.dimension( 0 );
   
   
   auto samples = kspace.samplesForView( 0 );
   fftshift( samples, _n0, _n1 );

   // prepare input
   fftw_complex * in = fftw_alloc_complex( samples.size( ) );
   fftw_complex * out = fftw_alloc_complex( samples.size( ) );
   for( auto i = 0; i < samples.size( ); ++i )
   {
      in[ i ][0] = samples.at( i )._asComplexFloat.real( );
      in[ i ][1] = samples.at( i )._asComplexFloat.imag( );
   }
   _in = in;
   _out = out;

}


void
KSpaceFFTTransform::postprocess( KSpace & kspace )
{
   fftw_complex * out = static_cast<fftw_complex *>( _out );
   SampleData samples( _n0 * _n1 );
   for( auto i = 0; i < samples.size( ); ++i ) {
      samples[ i ]._asComplexFloat = std::complex<float>( out[ i ][ 0 ], out[ i ][ 1 ] );
   }

   fftshift( samples, _n0, _n1 );

   kspace.setSample( std::move( samples ) );
}


bool
KSpaceFFTTransform::transform( KSpace & kspace )
{
   fftw_plan plan =  fftw_plan_dft_2d( _n0, _n1,
                                       static_cast<fftw_complex *>( _in ), static_cast<fftw_complex *>( _out ),
                                       FFTW_BACKWARD, FFTW_ESTIMATE
                                      );
   fftw_execute( plan );
   fftw_destroy_plan( plan );
   return true;
}


void
KSpaceFFTTransform::fftshift( SampleData & data, int n0, int n1 )
{
   for( auto i = 0 ; i < n0; ++i ) {
      std::rotate( data.begin( ) + i * n1, data.begin( ) + i * n1 + n1 / 2, data.begin( ) + i * n1 + n1 );
   }
   std::swap_ranges( data.begin( ), data.begin( ) + data.size( ) / 2, data.begin( ) + data.size( ) / 2 );
}
