/*=========================================================================

  Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   vtkITK
  Module:    $HeadURL: http://svn.slicer.org/Slicer4/trunk/Libs/vtkITK/vtkITKWin32Header.h $
  Date:      $Date: 2012-08-10 03:44:18 +0800 (周五, 10 八月 2012) $
  Version:   $Revision: 20740 $

==========================================================================*/

///  vtkITKWin32Header - manage Windows system differences
/// 
/// The vtkITKWin32Header captures some system differences between Unix
/// and Windows operating systems. 

#ifndef __vtkITKWin32Header_h
#define __vtkITKWin32Header_h

#include <vtkITKConfigure.h>

#if defined(WIN32) && !defined(VTKITK_STATIC)
#if defined(vtkITK_EXPORTS)
#define VTK_ITK_EXPORT __declspec( dllexport ) 
#else
#define VTK_ITK_EXPORT __declspec( dllimport ) 
#endif
#else
#define VTK_ITK_EXPORT
#endif

#endif
