/*=========================================================================

  Program:   Diffusion Applications
  Module:    $HeadURL: http://svn.slicer.org/Slicer4/trunk/Modules/CLI/ResampleDTIVolume/itkDiffusionTensor3DWrite.txx $
  Language:  C++
  Date:      $Date: 2012-06-20 01:53:37 +0800 (周三, 20 六月 2012) $
  Version:   $Revision: 20406 $

  Copyright (c) Brigham and Women's Hospital (BWH) All Rights Reserved.

  See License.txt or http://www.slicer.org/copyright/copyright.txt for details.

==========================================================================*/
#ifndef __itkDiffusionTensor3DWrite_txx
#define __itkDiffusionTensor3DWrite_txx

#include "itkDiffusionTensor3DWrite.h"

#if ITK_VERSION_MAJOR < 4
/* This is a bug in ITKv3 that can cause conflicts when linking ITKv3 and teem with an executable */
/* ITKv4 has mangled the items taken from teem so that they do not conflict*/
#else
/* Thus requiring explicit inclusion of teem headers  (or better yet, using the ITK mangled version of these enumerations)*/
#ifndef NRRD_SPACE_MAX
#include <NrrdIO.h>
#endif
#endif

namespace itk
{

template <class TData>
DiffusionTensor3DWrite<TData>
::DiffusionTensor3DWrite()
{
  m_NumberOfThreads = 0;
}

template <class TData>
void
DiffusionTensor3DWrite<TData>
::SetMetaDataDictionary( DictionaryType dic )
{
  m_MetaDataDictionary = dic;
  this->Modified();
}

template <class TData>
void
DiffusionTensor3DWrite<TData>
::SetSpace(int space)
{
  if( space < 0 || space > NRRD_SPACE_MAX + 1 )
    {
    return;
    }
  DictionaryType::ConstIterator itr = m_MetaDataDictionary.Begin();
  DictionaryType::ConstIterator end = m_MetaDataDictionary.End();
  while( itr != end )
    {
    // Get Measurement Frame
    itk::MetaDataObjectBase::Pointer entry = itr->second;
    MetaDataIntType::Pointer         entryvalue
      = dynamic_cast<MetaDataIntType *>( entry.GetPointer() );
    if( entryvalue )
      {
      int pos = itr->first.find( "space" );
      if( pos != -1 )
        {
        entryvalue->SetMetaDataObjectValue( nrrdSpace->str[space] );
        }
      }
    ++itr;
    }
}

template <class TData>
void
DiffusionTensor3DWrite<TData>
::SetMeasurementFrame( Matrix<double, 3, 3> measurementFrame )
{
  DictionaryType::ConstIterator itr = m_MetaDataDictionary.Begin();
  DictionaryType::ConstIterator end = m_MetaDataDictionary.End();

  while( itr != end )
    {
    // Get Measurement Frame
    itk::MetaDataObjectBase::Pointer  entry = itr->second;
    MetaDataDoubleVectorType::Pointer entryvalue
      = dynamic_cast<MetaDataDoubleVectorType *>( entry.GetPointer() );
    if( entryvalue )
      {
      int pos = itr->first.find( "NRRD_measurement frame" );
      if( pos != -1 )
        {
        DoubleVectorType tagvalue;
        tagvalue.resize( 3 );
        for( int i = 0; i < 3; i++ )
          {
          tagvalue[i].resize( 3 );
          for( int j = 0; j < 3; j++ )
            {
//            tagvalue[ i ][ j ] = measurementFrame[ i ][ j ] ;
            tagvalue[i][j] = measurementFrame[j][i];
            }
          }
        entryvalue->SetMetaDataObjectValue( tagvalue );
        }
      }
    ++itr;
    }
}

template <class TData>
int
DiffusionTensor3DWrite<TData>
::Update( const char* output )
{
  m_Input->SetMetaDataDictionary( m_MetaDataDictionary );
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetInput( m_Input );
  writer->SetFileName( output );
  writer->UseCompressionOn();
  writer->SetNumberOfThreads(m_NumberOfThreads);
  try
    {
    writer->Update();
    return 0;
    }
  catch( itk::ExceptionObject excep )
    {
    std::cerr
    << "DiffusionTensor3DWrite::Write: exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    return -1;
    }
  return 0;
}

} // end namespace itk

#endif
