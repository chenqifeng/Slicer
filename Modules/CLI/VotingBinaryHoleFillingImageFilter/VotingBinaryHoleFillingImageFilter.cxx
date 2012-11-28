/*=========================================================================

  Program:   Slicer
  Language:  C++
  Module:    $HeadURL: http://svn.slicer.org/Slicer4/trunk/Modules/CLI/VotingBinaryHoleFillingImageFilter/VotingBinaryHoleFillingImageFilter.cxx $
  Date:      $Date: 2011-12-07 04:49:19 +0800 (周三, 07 十二月 2011) $
  Version:   $Revision: 18864 $

  Copyright (c) Brigham and Women's Hospital (BWH) All Rights Reserved.

  See License.txt or http://www.slicer.org/copyright/copyright.txt for details.

==========================================================================*/

// This program is modified version of the
// VotingBinaryHoleFillImageFilter.cxx example that is distributed
// with the ITK.

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkVotingBinaryHoleFillingImageFilter.h"
#include "itkPluginFilterWatcher.h"

#include "VotingBinaryHoleFillingImageFilterCLP.h"

// Use an anonymous namespace to keep class types and function names
// from colliding when module is used as shared object module.  Every
// thing should be in an anonymous namespace except for the module
// entry point, e.g. main()
//
namespace
{

} // end of anonymous namespace

int main( int argc, char * argv[] )
{
  PARSE_ARGS;

  typedef   short InputPixelType;
  typedef   short OutputPixelType;

  typedef itk::Image<InputPixelType,  3> InputImageType;
  typedef itk::Image<OutputPixelType, 3> OutputImageType;

  typedef itk::ImageFileReader<InputImageType>  ReaderType;
  typedef itk::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( inputVolume.c_str() );
  writer->SetFileName( outputVolume.c_str() );

  typedef itk::VotingBinaryHoleFillingImageFilter<
    InputImageType, OutputImageType>  FilterType;

  FilterType::Pointer      filter = FilterType::New();
  itk::PluginFilterWatcher watcher(filter, "Voting Binary Hole Filling",
                                   CLPProcessInformation);

  InputImageType::SizeType indexRadius;

  indexRadius[0] = radius[0]; // radius along x
  indexRadius[1] = radius[1]; // radius along y
  indexRadius[2] = radius[2]; // radius along z

  filter->SetRadius( indexRadius );

  filter->SetBackgroundValue( background );
  filter->SetForegroundValue( foreground );
  filter->SetMajorityThreshold( majorityThreshold );

  filter->SetInput( reader->GetOutput() );
  writer->SetInput( filter->GetOutput() );
  writer->SetUseCompression(1);
  writer->Update();

  return EXIT_SUCCESS;
}