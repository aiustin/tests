//
//  kspacetransform.h
//  mrd2bmp
//

#ifndef _KSPACETRANSFORM_H_
#define _KSPACETRANSFORM_H_

#include "kspace.h"

/*********************************************************************
 *
 * KSpaceTransform
 * - applies a transformation to a KSpace.
 * - kspace can be used to retrieve the result
 * - added support only for SDT_ComplexFloat
 *
 *********************************************************************/


class KSpaceTransform
{
public:
   KSpaceTransform( const KSpace & kspace ) : _kspace( kspace ) { assert( kspace.sampleDataType( ) == SampleDataType::SDT_ComplexFloat ); }
   KSpaceTransform( KSpace && kspace ) : _kspace( std::move( kspace ) ) { assert( kspace.sampleDataType( ) == SampleDataType::SDT_ComplexFloat ); }
   virtual ~KSpaceTransform( ) { }
   
   bool apply( );
   
   bool isTransformed( ) const { return _transformed; }
   KSpace kspace( ) const { return _kspace; }
   
private:
   virtual void preprocess( KSpace & kspace ) =0;
   virtual bool transform( KSpace & kspace ) =0;
   virtual void postprocess( KSpace & kspace ) =0;
private:
   bool _transformed = false;
   KSpace _kspace;
};


/*********************************************************************
 *
 * KSpaceFFTTransform
 * - a FFT customization
 *
 *********************************************************************/


class KSpaceFFTTransform : public KSpaceTransform
{
public:
   using KSpaceTransform::KSpaceTransform;
   virtual ~KSpaceFFTTransform( );

private:
   virtual void preprocess( KSpace & kspace ) override;
   virtual bool transform( KSpace & kspace ) override;
   virtual void postprocess( KSpace & kspace ) override;
   
   void fftshift( SampleData & data, int n0, int n1 );
   
private:
   int _n0 = 0;
   int _n1 = 0;
   void * _in = nullptr;
   void * _out = nullptr;
};


#endif /* _KSPACETRANSFORM_H_ */
