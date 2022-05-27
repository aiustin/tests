//
//  kspacerepresentation.cpp
//  mrd2bmp
//

#include "kspacerepresentation.h"

#include "EasyBMP.h"


void
KSpaceRepresentation::generate( BMP & bmp, Type t )
{
   bmp.SetSize( _kspace.sampleSize( ), _kspace.samplesPerView( ) );
   bmp.SetBitDepth( 24 );

   auto & representation_data = _representations[ t ];
   switch( kspace( ).sampleDataType( ) )
   {
      case SampleDataType::SDT_Char:
      case SampleDataType::SDT_Int:
      case SampleDataType::SDT_Float:
      case SampleDataType::SDT_ComplexChar:
      case SampleDataType::SDT_ComplexInt:
         assert( false );
         break;
      case SampleDataType::SDT_ComplexFloat:
         if ( representation_data.empty( ) )
         {
            auto samples = kspace( ).samplesForView( 0 );
            std::transform( samples.begin( ), samples.end( ), std::back_inserter( representation_data ), t == KSR_Magnitude ? SampleDataValue::magnitudef : SampleDataValue::phasef );
            normalize( representation_data );
            generate( bmp, dataToColour( representation_data ) );
         }
         break;
      default:
         assert( false );
         break;
   }
}


void
KSpaceRepresentation::generate( BMP & bmp, const std::vector<unsigned char> & data )
{
   assert( data.size( ) == bmp.TellHeight( ) * bmp.TellWidth( ) );
   for( auto y = 0; y < bmp.TellHeight( ); ++y )
   {
      for( auto x = 0; x < bmp.TellWidth( ); ++x )
      {
         const auto colour = data[ y * bmp.TellWidth( ) + x ];
         bmp.SetPixel( x, y, RGBApixel{ colour, colour, colour, 0 } );
      }
   }
}
