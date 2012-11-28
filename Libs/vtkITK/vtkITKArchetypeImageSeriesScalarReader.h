/*=========================================================================

  Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   vtkITK
  Module:    $HeadURL: http://svn.slicer.org/Slicer4/trunk/Libs/vtkITK/vtkITKArchetypeImageSeriesScalarReader.h $
  Date:      $Date: 2012-08-10 03:44:18 +0800 (周五, 10 八月 2012) $
  Version:   $Revision: 20740 $

==========================================================================*/

#ifndef __vtkITKArchetypeImageSeriesScalarReader_h
#define __vtkITKArchetypeImageSeriesScalarReader_h

#include "vtkITKArchetypeImageSeriesReader.h"

#include "itkImageFileReader.h"

class VTK_ITK_EXPORT vtkITKArchetypeImageSeriesScalarReader : public vtkITKArchetypeImageSeriesReader
{
 public:
  static vtkITKArchetypeImageSeriesScalarReader *New();
  vtkTypeRevisionMacro(vtkITKArchetypeImageSeriesScalarReader,vtkITKArchetypeImageSeriesReader);
  void PrintSelf(ostream& os, vtkIndent indent);

 protected:
  vtkITKArchetypeImageSeriesScalarReader();
  ~vtkITKArchetypeImageSeriesScalarReader();

  void ExecuteData(vtkDataObject *data);
  static void ReadProgressCallback(itk::ProcessObject* obj,const itk::ProgressEvent&, void* data);
  /// private:
};

#endif
