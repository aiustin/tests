//
//  mrdfile.cpp
//  mrd2bmp
//

#include "mrdfile.h"

#include <unordered_map>
#include <stdio.h>
#include <cassert>


static const unsigned short MASK_SIGNED_CHAR = 0x0001;
static const unsigned short MASK_INT         = 0x0003;
static const unsigned short MASK_FLOAT       = 0x0005;

static const unsigned short MASK_REAL        = 0x0000;
static const unsigned short MASK_COMPLEX     = 0x0010;

static const unsigned short MASK_BASE_TYPE   = 0x000f;
static const unsigned short MASK_EXT_TYPE    = 0x00f0;


static SampleDataType
dataTypeCodeToSampleDataType( unsigned short dtc )
{
   const bool is_complex = ( dtc & MASK_EXT_TYPE ) == MASK_COMPLEX;
   if ( ( dtc & MASK_BASE_TYPE ) == MASK_SIGNED_CHAR ) {
      return is_complex ? SampleDataType::SDT_ComplexChar : SampleDataType::SDT_Char;
   }
   if ( ( dtc & MASK_BASE_TYPE ) == MASK_INT ) {
      return is_complex ? SampleDataType::SDT_ComplexInt : SampleDataType::SDT_Int;
   }
   if ( ( dtc & MASK_BASE_TYPE ) == MASK_FLOAT ) {
      return is_complex ? SampleDataType::SDT_ComplexFloat : SampleDataType::SDT_Float;
   }
   assert( false );
   return SampleDataType::SDT_ComplexFloat;
}


struct MRDHeader
{
   bool isValid( )      const;
   bool isComplex( )    const { return ( _dataTypeCode & MASK_EXT_TYPE ) == MASK_COMPLEX; }
   bool isReal( )       const { return ( _dataTypeCode & MASK_EXT_TYPE ) == MASK_REAL; }
   bool isChar( )       const { return ( _dataTypeCode & MASK_BASE_TYPE ) == MASK_SIGNED_CHAR; }
   bool isInt( )        const { return ( _dataTypeCode & MASK_BASE_TYPE ) == MASK_INT; }
   bool isFloat( )      const { return ( _dataTypeCode & MASK_BASE_TYPE ) == MASK_FLOAT; }
   int dataTypeSize( )  const { return ( isChar( ) ? 1 : ( isInt( ) ? 2 : 4 ) ) * ( isComplex( ) ? 2 : 1 ); }
   
   
   int _dimensions[ 4 ] = { 0 };
   char _undefined1[ 2 ] = { 0 };
   unsigned short _dataTypeCode = 0;
   char _undefined2[ 228 ] = { 0 };
   int _dimension5 = 0;
   int _dimension6 = 0;
};


static_assert( sizeof(int) == 4, "sizeof(int) != 4" );
static_assert( sizeof(short) == 2, "sizeof(short) != 2" );
static_assert( sizeof(float) == 4, "sizeof(float) != 4" );
static_assert( sizeof(MRDHeader) == 256, "sizeof(MRDHeader) != 256" );





bool
MRDHeader::isValid( ) const
{
   //Valid values for the purpose of this exercise
   const auto base_type = _dataTypeCode & MASK_BASE_TYPE;
   const auto ext_type = _dataTypeCode & MASK_EXT_TYPE;
   return   _dimensions[ 0 ] >= 1 &&
            _dimensions[ 1 ] >= 1 &&
            _dimensions[ 2 ] == 1 &&
            _dimensions[ 3 ] == 1 &&
/* Looks like dimension5 & 6 are 0 */
            _dimension5 == 0      &&
            _dimension6 == 0      &&
/**/
            ( base_type == MASK_SIGNED_CHAR   ||
              base_type == MASK_INT           ||
              base_type == MASK_FLOAT
             ) &&
            ( ext_type == MASK_REAL ||
              ext_type == MASK_COMPLEX
             )
   ;
}



/*******************************************************************************************************
 *** MRDFileImplPrivate
 * - assumes the Endianness of the app is the same with the one of the file
 * - can be extended to return much more detailed error info
 ******************************************************************************************************/

struct MRDFileImplPrivate : MRDFile::MRDFileImpl
{
   MRDFileImplPrivate( const std::string & fn ) : _fileName( fn ) { }
   ~MRDFileImplPrivate( ) { close( ); }
   
   bool open( );
   void close( );
   bool read( );
   
   bool isOpen( ) const { return _handle; }
   
   const std::string _fileName;
   FILE * _handle = nullptr;
   
   MRDHeader _header;
   char _text[256] = { 0 };
   std::unique_ptr<char[]> _data;
   unsigned long _dataSize = 0;
   std::unordered_map<std::string, std::string> _parameters;
};



bool
MRDFileImplPrivate::open( )
{
   assert( ! _handle );
   if ( _handle ) {
      return false;
   }
#ifdef __APPLE__
   return ( _handle = fopen( _fileName.c_str( ), "rb" ) );
#else
   fopen_s( &_handle, _fileName.c_str( ), "rb" );
   return _handle;
#endif
}


void
MRDFileImplPrivate::close( )
{
   if ( isOpen( ) )
   {
      fclose( _handle );
      _handle = nullptr;
   }
}


bool
MRDFileImplPrivate::read( )
{
   assert( isOpen( ) );
   if ( ! isOpen( ) ) {
      return false;
   }
   
   //Header
   auto read_size = fread( &_header, sizeof(_header), 1, _handle );
   assert( read_size );
   if ( ! read_size ) {
      return false;
   }
   assert( _header.isValid( ) );
   
   //Text
   read_size = fread( &_text, sizeof(_text), 1, _handle );
   assert( read_size );
   if ( ! read_size ) {
      return false;
   }
   
   //Data
   /* Can't take _dimension5 & _dimension6 into account as they seem to be null */
   _dataSize = (unsigned long) _header._dimensions[ 0 ] * _header._dimensions[ 1 ] * _header._dimensions[ 2 ] * _header._dimensions[ 3 ] /* * _header._dimension5 * _header._dimension6 */;
   _dataSize *= _header.dataTypeSize( );
   _data = std::make_unique<char[]>( _dataSize );
   read_size = fread( _data.get( ), _dataSize, 1, _handle );
   assert( read_size );
   if ( ! read_size ) {
      return false;
   }
   
   //Parameters
   // Skip those for now as we don't use them

   return true;
}



/*******************************************************************************************************
 *** MRDFile
 ******************************************************************************************************/


MRDFile::MRDFile( const std::string & fileName ) :
   _d( std::make_unique<MRDFileImplPrivate>( fileName ) )
{
}


bool
MRDFile::load( )
{
   if ( _loaded ) {
      return true;
   }
   auto mrdfile_impl = impl<MRDFileImplPrivate>( );
   if ( ! mrdfile_impl->open( ) ) {
      return false;
   }
   
   if ( mrdfile_impl->read( ) && mrdfile_impl->_header.isValid( ) )
   {
      _mrdData._dimensions[ 0 ] = mrdfile_impl->_header._dimensions[ 0 ];
      _mrdData._dimensions[ 1 ] = mrdfile_impl->_header._dimensions[ 1 ];
      _mrdData._dimensions[ 2 ] = mrdfile_impl->_header._dimensions[ 2 ];
      _mrdData._dimensions[ 3 ] = mrdfile_impl->_header._dimensions[ 3 ];
      _mrdData._dimensions[ 4 ] = 1;
      _mrdData._dimensions[ 5 ] = 1;
      /*
      _mrdData._dimensions[ 4 ] = mrdfile_impl->_header._dimension5;
      _mrdData._dimensions[ 5 ] = mrdfile_impl->_header._dimension6;
       */
      _mrdData._sdt = dataTypeCodeToSampleDataType( mrdfile_impl->_header._dataTypeCode );
      
      // Copy data from mrdfile_impl->_data into _mrdData._data
      unsigned long data_size = 0;
      switch ( _mrdData._sdt )
      {
         case SampleDataType::SDT_Char:
            data_size = mrdfile_impl->_dataSize / sizeof( char );
            _mrdData._data.resize( data_size );
            std::generate( _mrdData._data.begin( ), _mrdData._data.end( ),
                          [src_data = reinterpret_cast<const char *>( mrdfile_impl->_data.get( ) ), i = 0]( ) mutable -> SampleDataValue { return SampleDataValue( src_data[ i++ ] ); } );
            break;
         case SampleDataType::SDT_Int:
            data_size = mrdfile_impl->_dataSize / sizeof( short );
            _mrdData._data.resize( data_size );
            std::generate( _mrdData._data.begin( ), _mrdData._data.end( ),
                          [src_data = reinterpret_cast<const short *>( mrdfile_impl->_data.get( ) ), i = 0]( ) mutable -> SampleDataValue { return SampleDataValue( src_data[ i++ ] ); } );
            break;
         case SampleDataType::SDT_Float:
            data_size = mrdfile_impl->_dataSize / sizeof( float );
            _mrdData._data.resize( data_size );
            std::generate( _mrdData._data.begin( ), _mrdData._data.end( ),
                          [src_data = reinterpret_cast<const float *>( mrdfile_impl->_data.get( ) ), i = 0]( ) mutable -> SampleDataValue { return SampleDataValue( src_data[ i++ ] ); } );
            break;
         case SampleDataType::SDT_ComplexChar:
            data_size = mrdfile_impl->_dataSize / sizeof( char );
            _mrdData._data.resize( data_size );
            std::generate( _mrdData._data.begin( ), _mrdData._data.end( ),
                          [src_data = reinterpret_cast<const char *>( mrdfile_impl->_data.get( ) ), i = 0]( ) mutable -> SampleDataValue { auto v = SampleDataValue( src_data[ i * 2 ], src_data[ i * 2 + 1] ); ++i;  return v; } );
            break;
         case SampleDataType::SDT_ComplexInt:
            data_size = mrdfile_impl->_dataSize / sizeof( short );
            _mrdData._data.resize( data_size );
            std::generate( _mrdData._data.begin( ), _mrdData._data.end( ),
                          [src_data = reinterpret_cast<const short *>( mrdfile_impl->_data.get( ) ), i = 0]( ) mutable -> SampleDataValue { auto v = SampleDataValue( src_data[ i * 2 ], src_data[ i * 2 + 1] ); ++i;  return v; } );
            break;
         case SampleDataType::SDT_ComplexFloat:
         {
            data_size = mrdfile_impl->_dataSize / ( sizeof( float ) * 2 );
            _mrdData._data.resize( data_size );
            // "For this optimisation to work SampleDataValue has to map exactly onto the file data format"
            if ( sizeof( SampleDataValue ) == 2 * sizeof( float ) )
            {
               void * dest = reinterpret_cast<void*>( _mrdData._data.data( ) );
               std::memmove( dest, mrdfile_impl->_data.get( ), mrdfile_impl->_dataSize);
            }
            else {
               std::generate( _mrdData._data.begin( ), _mrdData._data.end( ),
                             [src_data = reinterpret_cast<const float *>( mrdfile_impl->_data.get( ) ), i = 0]( ) mutable -> SampleDataValue { auto v = SampleDataValue( src_data[ i * 2 ], src_data[ i * 2 + 1] ); ++i;  return v; } );
            }
         }
            break;
         default:
            assert( false );
            break;
      }
      _loaded = data_size;
   }
   
   mrdfile_impl->close( );
   return _loaded;
}


