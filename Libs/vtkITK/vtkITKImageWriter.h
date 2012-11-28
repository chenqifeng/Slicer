/*=========================================================================

  Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   vtkITK
  Module:    $HeadURL: http://svn.slicer.org/Slicer4/trunk/Libs/vtkITK/vtkITKImageWriter.h $
  Date:      $Date: 2012-09-19 08:49:07 +0800 (周三, 19 九月 2012) $
  Version:   $Revision: 21009 $

==========================================================================*/

#ifndef __vtkITKImageWriter_h
#define __vtkITKImageWriter_h

#include "vtkProcessObject.h"
#include "vtkImageData.h"
#include "vtkObjectFactory.h"
#include "vtkMatrix4x4.h"

#include "vtkITK.h"
#include "itkImageIOBase.h"

class vtkStringArray;

class VTK_ITK_EXPORT vtkITKImageWriter : public vtkProcessObject
{
public:
  static vtkITKImageWriter *New();
  vtkTypeRevisionMacro(vtkITKImageWriter,vtkProcessObject);
  void PrintSelf(ostream& os, vtkIndent indent);
  
  /// 
  /// Specify file name for the image file. You should specify either
  /// a FileName or a FilePrefix. Use FilePrefix if the data is stored 
  /// in multiple files.
  void SetFileName(const char *);

  char *GetFileName() {
    return FileName;
  }

  /// 
  /// use compression if possible
  vtkGetMacro (UseCompression, int);
  vtkSetMacro (UseCompression, int);
  vtkBooleanMacro(UseCompression, int);

  /// 
  /// Set/Get the input object from the image pipeline.
  void SetInput(vtkImageData *input);
  vtkImageData *GetInput();

  /// 
  /// Set/Get the ImageIO class name.
  vtkGetStringMacro (ImageIOClassName);
  vtkSetStringMacro (ImageIOClassName);

  /// 
  /// The main interface which triggers the writer to start.
  void Write();

  /// Set orienation matrix
  void SetRasToIJKMatrix( vtkMatrix4x4* mat) {
    RasToIJKMatrix = mat;
  }

  /// Set orienation matrix
  void SetMeasurementFrameMatrix( vtkMatrix4x4* mat) {
    MeasurementFrameMatrix = mat;
  }

protected:
  vtkITKImageWriter();
  ~vtkITKImageWriter();

  char *FileName;
  vtkMatrix4x4* RasToIJKMatrix;
  vtkMatrix4x4* MeasurementFrameMatrix;
  int UseCompression;
  char* ImageIOClassName;

private:
  vtkITKImageWriter(const vtkITKImageWriter&);  /// Not implemented.
  void operator=(const vtkITKImageWriter&);  /// Not implemented.
};

//vtkStandardNewMacro(vtkITKImageWriter)

#endif
