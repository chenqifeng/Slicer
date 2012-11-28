/*=========================================================================

  Program:   Diffusion Applications
  Module:    $HeadURL: http://svn.slicer.org/Slicer4/trunk/Modules/CLI/ResampleDTIVolume/itkDiffusionTensor3DExtended.txx $
  Language:  C++
  Date:      $Date: 2012-02-02 14:52:52 +0800 (周四, 02 二月 2012) $
  Version:   $Revision: 19197 $

  Copyright (c) Brigham and Women's Hospital (BWH) All Rights Reserved.

  See License.txt or http://www.slicer.org/copyright/copyright.txt for details.

==========================================================================*/
#ifndef __itkDiffusionTensor3DExtended_txx
#define __itkDiffusionTensor3DExtended_txx

#include "itkDiffusionTensor3DExtended.h"

namespace itk
{

template <class T>
DiffusionTensor3DExtended<T>
::DiffusionTensor3DExtended( const Superclass & tensor )
{
  for( int i = 0; i < 6; i++ )
    {
    this->SetElement( i, tensor.GetElement( i ) );
    }
}

template <class T>
typename DiffusionTensor3DExtended<T>::MatrixType
DiffusionTensor3DExtended<T>
::GetTensor2Matrix()
{
  MatrixType matrix;

  for( int i = 0; i < 3; i++ )
    {
    for( int j = 0; j < 3; j++ )
      {
      matrix[i][j] = ( *this )( i, j );
      }
    }
  return matrix;
}

template <class T>
template <class C>
void
DiffusionTensor3DExtended<T>
::SetTensorFromMatrix( Matrix<C, 3, 3> matrix )
{
  for( int i = 0; i < 3; i++ )
    {
    for( int j = i; j < 3; j++ )
      {
      ( *this )( i, j ) = static_cast<T>( matrix[i][j] );
      }
    }
}

template <class T>
template <class C>
DiffusionTensor3DExtended<T>
::operator DiffusionTensor3DExtended<C> const ()
  {
  DiffusionTensor3DExtended<C> tmp;
  for( int i = 0; i < 6; i++ )
    {
    tmp.SetElement( i, ( C ) ( this->GetElement( i ) ) );
    }
  return tmp;
  }

} // end namespace itk
#endif
