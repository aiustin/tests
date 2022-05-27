//
//  main.cpp
//  mrd2bmp
//

#include <iostream>

#include "kspace/mrdfile.h"
#include "kspace/kspace.h"
#include "kspace/kspacetransform.h"
#include "kspace/kspacerepresentation.h"

#include "EasyBMP.h"


int main(int argc, const char * argv[ ] )
{
   if ( argc < 2 )
   {
      std::cout << "Not enough arguments. e.g.: mrd2bmp <mrd_file_path>" << std::endl;
      return 1;
   }
   MRDFile mrd_file( argv[ 1 ] );
   
   if ( mrd_file.load( ) )
   {
      std::cout << "Dimensions: ";
      for( auto i : mrd_file.dimensions( ) )
      {
         std::cout << i << " ";
      }
      std::cout << std::endl;
   }
   
   KSpace k_space = KSpace::fromFile( mrd_file );
   
   BMP bmp;
   
   KSpaceRepresentation orig_rep( k_space );
   orig_rep.generate( bmp, KSpaceRepresentation::KSR_Magnitude );
   bmp.WriteToFile( "magnitudes.bmp" );
   orig_rep.generate( bmp, KSpaceRepresentation::KSR_Phase );
   bmp.WriteToFile( "phases.bmp" );
   
   
   KSpaceFFTTransform fft_transform( k_space );
   bool ok = fft_transform.apply( );
   assert( ok );
   KSpaceRepresentation fft_rep( fft_transform.kspace( ) );
   fft_rep.generate( bmp, KSpaceRepresentation::KSR_Magnitude );
   bmp.WriteToFile( "magnitudes_fftw.bmp" );
   fft_rep.generate( bmp, KSpaceRepresentation::KSR_Phase );
   bmp.WriteToFile( "phases_fftw.bmp" );
   
   return 0;
}
