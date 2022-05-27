//
//  datatypedefs.h
//  mrd2bmp
//

#ifndef _DATATYPEDEFS_H_
#define _DATATYPEDEFS_H_

#include <complex>
#include <array>
#include <vector>


enum class SampleDataType
{
   SDT_Char          ,
   SDT_Int           ,
   SDT_Float         ,
   SDT_ComplexChar   ,
   SDT_ComplexInt    ,
   SDT_ComplexFloat  ,
};


union SampleDataValue
{
   explicit SampleDataValue( ) : _asComplexFloat { 0.0f,  0.0f } { }
   explicit SampleDataValue( float r, float i ) : _asComplexFloat { r,  i } { }
   explicit SampleDataValue( short r, short i ) : _asComplexInt { r,  i } { }
   explicit SampleDataValue( char r, char i ) : _asComplexChar { r,  i } { }
   explicit SampleDataValue( float f ) : _asFloat { f } { }
   explicit SampleDataValue( short i ) : _asInt { i } { }
   explicit SampleDataValue( char c ) : _asChar { c } { }
   
   char _asChar;
   short _asInt;
   float _asFloat;
   std::complex<char> _asComplexChar;
   std::complex<short> _asComplexInt;
   std::complex<float> _asComplexFloat;
   
   static float magnitudef( const SampleDataValue & sdv ) { return std::abs( sdv._asComplexFloat ); }
   static float phasef( const SampleDataValue & sdv ) { return std::atan2f( sdv._asComplexFloat.imag( ), sdv._asComplexFloat.real( ) ); }
};

using SampleData = std::vector<SampleDataValue>;
using SampleDataDimensions = std::array<int, 6>;


#endif /* _DATATYPEDEFS_H_ */
