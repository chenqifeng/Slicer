/*=========================================================================

  Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   vtkITK
  Module:    $HeadURL: http://svn.slicer.org/Slicer4/trunk/Libs/vtkITK/vtkITKImageToImageFilterF2F.h $
  Date:      $Date: 2012-09-19 08:49:07 +0800 (周三, 19 九月 2012) $
  Version:   $Revision: 21009 $

==========================================================================*/

#ifndef __vtkITKImageToImageFilterF2F_h
#define __vtkITKImageToImageFilterF2F_h


#include "vtkITKImageToImageFilter.h"
#include "vtkImageToImageFilter.h"
#include "itkImageToImageFilter.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "vtkITKUtility.h"


class VTK_ITK_EXPORT vtkITKImageToImageFilterF2F : public vtkITKImageToImageFilter
{
public:
  vtkTypeMacro(vtkITKImageToImageFilterF2F,vtkITKImageToImageFilter);
  static vtkITKImageToImageFilterF2F* New() { return 0; };
  void PrintSelf(ostream& os, vtkIndent indent)
  {
    Superclass::PrintSelf ( os, indent );
    os << m_Filter;
  };

protected:
  
  /// To/from ITK
  typedef itk::Vector<float,2> InputImagePixelType;
  typedef float OutputImagePixelType;
  typedef itk::Image<InputImagePixelType, 3> InputImageType;
  typedef itk::Image<OutputImagePixelType, 3> OutputImageType;

  typedef itk::VTKImageImport<InputImageType> ImageImportType;
  typedef itk::VTKImageExport<OutputImageType> ImageExportType;
  ImageImportType::Pointer itkImporter;
  ImageExportType::Pointer itkExporter;

  typedef itk::ImageToImageFilter<InputImageType,OutputImageType> GenericFilterType;
  GenericFilterType::Pointer m_Filter;

  vtkITKImageToImageFilterF2F ( GenericFilterType* filter )
  {
    /// Need an import, export, and a ITK pipeline
    m_Filter = filter;
    this->itkImporter = ImageImportType::New();
    this->itkExporter = ImageExportType::New();
    ConnectPipelines(this->vtkExporter, this->itkImporter);
    ConnectPipelines(this->itkExporter, this->vtkImporter);
    this->LinkITKProgressToVTKProgress ( m_Filter );
    
    /// Set up the filter pipeline
    m_Filter->SetInput ( this->itkImporter->GetOutput() );
    this->itkExporter->SetInput ( m_Filter->GetOutput() );
    this->vtkCast->SetOutputScalarTypeToFloat();
  };

  ~vtkITKImageToImageFilterF2F()
  {
  };
  
private:
  vtkITKImageToImageFilterF2F(const vtkITKImageToImageFilterF2F&);  /// Not implemented.
  void operator=(const vtkITKImageToImageFilterF2F&);  /// Not implemented.
};

#endif



