//
//  mrdfile.h
//  mrd2bmp
//

#ifndef _MRDFILE_H_
#define _MRDFILE_H_


/*********************************************************************
 *
 * MRDFile
 * - opens and loads a MRD file
 *
 *********************************************************************/


#include "datatypedefs.h"

#include <string>
#include <complex>


class MRDFile
{
public:
   MRDFile( const std::string & fileName );
   inline ~MRDFile( ){ }
   
   MRDFile( const MRDFile& ) =delete;
   inline MRDFile( MRDFile&& other ) : _d( std::move( other._d ) ) { other._d.reset( ); }
   
   MRDFile& operator=( const MRDFile& ) =delete;
   inline MRDFile& operator=( MRDFile&& other ) { _d = std::move( other._d ); other._d.reset( ); return *this; }

   bool load( );
   inline bool isLoaded( ) const { return _loaded; }
   
   inline SampleDataDimensions   dimensions( )     const { return _mrdData._dimensions; }
   inline SampleDataType         sampleSataType( ) const { return _mrdData._sdt; }
   inline SampleData             data( )           const { return _mrdData._data; }
   
private:
   struct MRDData
   {
      SampleDataDimensions _dimensions;
      SampleDataType       _sdt;
      SampleData           _data;
   };
   
   MRDData _mrdData;
   
   
private:
   friend struct MRDFileImplPrivate;
   struct MRDFileImpl{ };
   
   // Just some convenience accessors
   template<typename T> const T* impl( ) const  { return static_cast<const T*>( _d.get( ) ); }
   template<typename T>       T* impl( )        { return static_cast<T*>( _d.get( ) ); }

   bool _loaded = false;
   std::unique_ptr<MRDFileImpl> _d;
};


#endif /* _MRDFILE_H_ */
