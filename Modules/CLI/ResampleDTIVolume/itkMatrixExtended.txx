/*=========================================================================

  Program:   Diffusion Applications
  Module:    $HeadURL: http://svn.slicer.org/Slicer4/trunk/Modules/CLI/ResampleDTIVolume/itkMatrixExtended.txx $
  Language:  C++
  Date:      $Date: 2012-02-02 14:52:52 +0800 (周四, 02 二月 2012) $
  Version:   $Revision: 19197 $

  Copyright (c) Brigham and Women's Hospital (BWH) All Rights Reserved.

  See License.txt or http://www.slicer.org/copyright/copyright.txt for details.

==========================================================================*/
#ifndef __itkMatrixExtended_txx
#define __itkMatrixExtended_txx

#include "itkMatrixExtended.h"

namespace itk
{

template <class T, unsigned int NRows, unsigned int NColumns>
MatrixExtended<T, NRows, NColumns>
::MatrixExtended() : Matrix<T, NRows, NColumns>()
{
}

template <class T, unsigned int NRows, unsigned int NColumns>
MatrixExtended<T, NRows, NColumns>
::MatrixExtended( const Self & matrix )
  : Matrix<T, NRows, NColumns>( Superclass( matrix ) )
{
}

template <class T, unsigned int NRows, unsigned int NColumns>
MatrixExtended<T, NRows, NColumns>
::MatrixExtended( const Superclass & matrix ) : Matrix<T, NRows, NColumns>( matrix )
{
}

template <class T, unsigned int NRows, unsigned int NColumns>
MatrixExtended<T, NRows, NColumns>
::MatrixExtended( const InternalMatrixType & matrix )
  : Matrix<T, NRows, NColumns>( matrix )
{
}

template <class T, unsigned int NRows, unsigned int NColumns>
template <class C, unsigned int NRowsC, unsigned int NColumnsC>
MatrixExtended<T, NRows, NColumns>
::operator MatrixExtended<C, NRowsC, NColumnsC> const ()
  {
  MatrixExtended<C, NRowsC, NColumnsC> tmp;
  for( unsigned int i = 0; i < NRows; i++ )
    {
    for( unsigned int j = 0; j < NColumns; j++ )
      {
      tmp[i][j] = ( C ) ( *this )[i][j];
      }
    }
  return tmp;
  }

template <class T, unsigned int NRows, unsigned int NColumns>
MatrixExtended<T, NRows, NColumns>
MatrixExtended<T, NRows, NColumns>
::operator=( const Self & matrix )
{
  for( unsigned int i = 0; i < NRows; i++ )
    {
    for( unsigned int j = 0; j < NColumns; j++ )
      {
      ( *this )[i][j] = matrix[i][j];
      }
    }
  return *this;
}

template <class T, unsigned int NRows, unsigned int NColumns>
MatrixExtended<T, NRows, NColumns>
MatrixExtended<T, NRows, NColumns>
::operator=( const Superclass & matrix )
{
  for( unsigned int i = 0; i < NRows; i++ )
    {
    for( unsigned int j = 0; j < NColumns; j++ )
      {
      ( *this )[i][j] = matrix[i][j];
      }
    }
  return *this;
}

template <class T, unsigned int NRows, unsigned int NColumns>
MatrixExtended<T, NRows, NColumns>
MatrixExtended<T, NRows, NColumns>
::operator=( const InternalMatrixType & matrix )
{
  for( unsigned int i = 0; i < NRows; i++ )
    {
    for( unsigned int j = 0; j < NColumns; j++ )
      {
      ( *this )[i][j] = matrix[i][j];
      }
    }
  return *this;
}

} // end namespace itk
#endif
