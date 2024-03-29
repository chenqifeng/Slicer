/*=========================================================================

  Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   vtkITK
  Module:    $HeadURL: http://svn.slicer.org/Slicer4/trunk/Libs/vtkITK/vtkITKImageToImageFilterUSUL.h $
  Date:      $Date: 2012-09-19 08:49:07 +0800 (周三, 19 九月 2012) $
  Version:   $Revision: 21009 $

==========================================================================*/

#ifndef __vtkITKImageToImageFilterUSUL_h
#define __vtkITKImageToImageFilterUSUL_h

#include "vtkImageToImageFilter.h"
#include "vtkITKImageToImageFilter.h"
#include "itkImageToImageFilter.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "vtkITKUtility.h"


class VTK_ITK_EXPORT vtkITKImageToImageFilterUSUL : public vtkITKImageToImageFilter
{
public:
  vtkTypeMacro(vtkITKImageToImageFilterUSUL,vtkITKImageToImageFilter);
  static vtkITKImageToImageFilterUSUL* New() { return 0; };
  void PrintSelf(ostream& os, vtkIndent indent)
  {
    Superclass::PrintSelf ( os, indent );
    os << m_Filter;
  };

protected:
  
  /// To/from ITK
  typedef itk::Image<unsigned short, 3> InputImageType;
  typedef itk::Image<unsigned long, 3> OutputImageType;

  typedef itk::VTKImageImport<InputImageType> ImageImportType;
  typedef itk::VTKImageExport<OutputImageType> ImageExportType;
  ImageImportType::Pointer itkImporter;
  ImageExportType::Pointer itkExporter;

  typedef itk::ImageToImageFilter<InputImageType,OutputImageType> FilterType;
  FilterType::Pointer m_Filter;

  vtkITKImageToImageFilterUSUL ( FilterType* filter )
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
    this->vtkCast->SetOutputScalarTypeToUnsignedShort();
  };

  ~vtkITKImageToImageFilterUSUL()
  {
  };
    
private:
  vtkITKImageToImageFilterUSUL(const vtkITKImageToImageFilterUSUL&);  /// Not implemented.
  void operator=(const vtkITKImageToImageFilterUSUL&);  /// Not implemented.
};

#endif
