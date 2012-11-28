/*=========================================================================

  Program:   Diffusion Applications
  Module:    $HeadURL: http://svn.slicer.org/Slicer4/trunk/Modules/CLI/ResampleDTIVolume/itkDiffusionTensor3DTransform.txx $
  Language:  C++
  Date:      $Date: 2012-02-02 14:52:52 +0800 (周四, 02 二月 2012) $
  Version:   $Revision: 19197 $

  Copyright (c) Brigham and Women's Hospital (BWH) All Rights Reserved.

  See License.txt or http://www.slicer.org/copyright/copyright.txt for details.

==========================================================================*/
#ifndef __itkDiffusionTensor3DTransform_txx
#define __itkDiffusionTensor3DTransform_txx

#include "itkDiffusionTensor3DTransform.h"

namespace itk
{

template <class TData>
DiffusionTensor3DTransform<TData>
::DiffusionTensor3DTransform()
{
  // Initialize the Measurement Frame to Identity
  m_MeasurementFrame.SetIdentity();
}

} // end namespace itk
#endif
