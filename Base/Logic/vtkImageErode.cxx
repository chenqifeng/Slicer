/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

=========================================================================auto=*/
#include "vtkImageErode.h"
#include "vtkObjectFactory.h"

#include <ctime>

//------------------------------------------------------------------------------
vtkCxxRevisionMacro(vtkImageErode, "$Revision: 20673 $");
vtkStandardNewMacro(vtkImageErode);

//----------------------------------------------------------------------------
// Description:
// Constructor sets default values
vtkImageErode::vtkImageErode()
{

    this->Background = 0;
    this->Foreground = 1;
    this->HandleBoundaries = 1;
    this->SetNeighborTo4();
}


//----------------------------------------------------------------------------
vtkImageErode::~vtkImageErode()
{

}

//----------------------------------------------------------------------------
// Description:
// This templated function executes the filter for any type of data.
// For every pixel in the foreground, if a neighbor is in the background,
// then the pixel becomes background.
template <class T>
static void vtkImageErodeExecute(vtkImageErode *self,
                     vtkImageData *inData, T *inPtr,
                     vtkImageData *outData,
                     int outExt[6], int id)
{
  // For looping though output (and input) pixels.
  int outMin0, outMax0, outMin1, outMax1, outMin2, outMax2;
  int outIdx0, outIdx1, outIdx2;
  vtkIdType inInc0, inInc1, inInc2;
  vtkIdType outInc0, outInc1, outInc2;
  T *inPtr0, *inPtr1, *inPtr2;
  T *outPtr0, *outPtr1, *outPtr2;
  int numComps, outIdxC;
  // For looping through hood pixels
  int hoodMin0, hoodMax0, hoodMin1, hoodMax1, hoodMin2, hoodMax2;
  int hoodIdx0, hoodIdx1, hoodIdx2;
  T *hoodPtr0, *hoodPtr1, *hoodPtr2;
  // For looping through the mask.
  unsigned char *maskPtr, *maskPtr0, *maskPtr1, *maskPtr2;
  vtkIdType maskInc0, maskInc1, maskInc2;
  // The extent of the whole input image
  int inImageMin0, inImageMin1, inImageMin2;
  int inImageMax0, inImageMax1, inImageMax2;
  // Other
  T backgnd = (T)(self->GetBackground());
  T foregnd = (T)(self->GetForeground());
  T pix;
  T *outPtr = (T*)outData->GetScalarPointerForExtent(outExt);
  unsigned long count = 0;
  unsigned long target;

  clock_t tStart, tEnd, tDiff;
  tStart = clock();

  // Get information to march through data
  inData->GetIncrements(inInc0, inInc1, inInc2); 
  self->GetInput()->GetWholeExtent(inImageMin0, inImageMax0, inImageMin1,
    inImageMax1, inImageMin2, inImageMax2);
  outData->GetIncrements(outInc0, outInc1, outInc2); 
  outMin0 = outExt[0];   outMax0 = outExt[1];
  outMin1 = outExt[2];   outMax1 = outExt[3];
  outMin2 = outExt[4];   outMax2 = outExt[5];
  numComps = outData->GetNumberOfScalarComponents();

  // Neighborhood around current voxel
  self->GetRelativeHoodExtent(hoodMin0, hoodMax0, hoodMin1, 
    hoodMax1, hoodMin2, hoodMax2);

  // Set up mask info
  maskPtr = (unsigned char *)(self->GetMaskPointer());
  self->GetMaskIncrements(maskInc0, maskInc1, maskInc2);

  // in and out should be marching through corresponding pixels.
  inPtr = (T *)(inData->GetScalarPointer(outMin0, outMin1, outMin2));

  target = (unsigned long)(numComps*(outMax2-outMin2+1)*
    (outMax1-outMin1+1)/50.0);
  target++;


  // loop through components
  for (outIdxC = 0; outIdxC < numComps; ++outIdxC)
    {
    // loop through pixels of output
    outPtr2 = outPtr;
    inPtr2 = inPtr;
    for (outIdx2 = outMin2; outIdx2 <= outMax2; outIdx2++)
      {
      outPtr1 = outPtr2;
      inPtr1 = inPtr2;
      for (outIdx1 = outMin1; 
        !self->AbortExecute && outIdx1 <= outMax1; outIdx1++)
        {
        if (!id) {
          if (!(count%target))
            self->UpdateProgress(count/(50.0*target));
          count++;
        }
        outPtr0 = outPtr1;
        inPtr0 = inPtr1;
        for (outIdx0 = outMin0; outIdx0 <= outMax0; outIdx0++)
          {
          pix = *inPtr0;
          // Default output equal to input
          *outPtr0 = pix;

          if (pix == foregnd)
            {
            // Loop through neighborhood pixels (kernel radius=1)
            // Note: input pointer marches out of bounds.
            hoodPtr2 = inPtr0 + inInc0*hoodMin0 + inInc1*hoodMin1 
              + inInc2*hoodMin2;
            maskPtr2 = maskPtr;
            for (hoodIdx2 = hoodMin2; hoodIdx2 <= hoodMax2; ++hoodIdx2)
              {
              hoodPtr1 = hoodPtr2;
              maskPtr1 = maskPtr2;
              for (hoodIdx1 = hoodMin1; hoodIdx1 <= hoodMax1;    ++hoodIdx1)
                {
                hoodPtr0 = hoodPtr1;
                maskPtr0 = maskPtr1;
                for (hoodIdx0 = hoodMin0; hoodIdx0 <= hoodMax0; ++hoodIdx0)
                  {
                  if (*maskPtr0)
                    {
                    // handle boundaries
                    if (outIdx0 + hoodIdx0 >= inImageMin0 &&
                      outIdx0 + hoodIdx0 <= inImageMax0 &&
                      outIdx1 + hoodIdx1 >= inImageMin1 &&
                      outIdx1 + hoodIdx1 <= inImageMax1 &&
                      outIdx2 + hoodIdx2 >= inImageMin2 &&
                      outIdx2 + hoodIdx2 <= inImageMax2)
                      {
                      // If the neighbor is backgnd, then
                      // set the output to backgnd
                      if (*hoodPtr0 == backgnd)
                        *outPtr0 = backgnd;
                      }
                    }
                  hoodPtr0 += inInc0;
                  maskPtr0 += maskInc0;
                  }//for0
                hoodPtr1 += inInc1;
                maskPtr1 += maskInc1;
                }//for1
              hoodPtr2 += inInc2;
              maskPtr2 += maskInc2;
              }//for2
            }//if
          inPtr0 += inInc0;
          outPtr0 += outInc0;
          }//for0
        inPtr1 += inInc1;
        outPtr1 += outInc1;
        }//for1
      inPtr2 += inInc2;
      outPtr2 += outInc2;
      }//for2
    inPtr++;
    outPtr++;
    }

  tEnd = clock();
  tDiff = tEnd - tStart;
  vtkDebugWithObjectMacro(self, << "tDiff:" << tDiff);
}

//----------------------------------------------------------------------------
// Description:
// This method is passed a input and output data, and executes the filter
// algorithm to fill the output from the input.
// It just executes a switch statement to call the correct function for
// the datas data types.
void vtkImageErode::ThreadedExecute(vtkImageData *inData, 
                    vtkImageData *outData,
                    int outExt[6], int id)
{
  void *inPtr = inData->GetScalarPointerForExtent(outExt);

  switch (inData->GetScalarType())
    {
  case VTK_DOUBLE:
    vtkImageErodeExecute(this, inData, (double *)(inPtr), 
      outData, outExt, id);
    break;
  case VTK_FLOAT:
    vtkImageErodeExecute(this, inData, (float *)(inPtr), 
      outData, outExt, id);
    break;
  case VTK_LONG:
    vtkImageErodeExecute(this, inData, (long *)(inPtr), 
      outData, outExt, id);
    break;
  case VTK_INT:
    vtkImageErodeExecute(this, inData, (int *)(inPtr), 
      outData, outExt, id);
    break;
  case VTK_UNSIGNED_INT:
    vtkImageErodeExecute(this, inData, (unsigned int *)(inPtr), 
      outData, outExt, id);
    break;
  case VTK_SHORT:
    vtkImageErodeExecute(this, inData, (short *)(inPtr), 
      outData, outExt, id);
    break;
  case VTK_UNSIGNED_SHORT:
    vtkImageErodeExecute(this, inData, (unsigned short *)(inPtr), 
      outData, outExt, id);
    break;
  case VTK_CHAR:
    vtkImageErodeExecute(this, inData, (char *)(inPtr), 
      outData, outExt, id);
    break;
  case VTK_UNSIGNED_CHAR:
    vtkImageErodeExecute(this, inData, (unsigned char *)(inPtr), 
      outData, outExt, id);
    break;
  default:
    vtkErrorMacro(<< "Execute: Unknown input ScalarType");
    return;
    }
}
