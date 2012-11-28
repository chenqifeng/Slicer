#ifndef __itkThresholdImageFilter2_txx
#define __itkThresholdImageFilter2_txx
#include "itkThresholdImageFilter2.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkObjectFactory.h
#include "itkProgressReporter.h"
namespace itk
{
  template <class TImage>
  ThresholdImageFilter2<TImage>::ThresholdImageFilter2()
  {
    m_Median = 0.5*(NumericTraits<PixelType>::max()+NumericTraits<PixelType>::NonpositiveMin());
	m_OutsideValue = NumericTraits<PixelType>::Zero;
	m_Lower = NumericTraits<PixelType>::NonpositiveMin();
	m_Upper = NumericTraits<PixelType>::max();
	m_Select=0;
	m_InsideValue=0.5*(NumericTraits<PixelType>::max()+NumericTraits<PixelType>::NonpositiveMin());
	this->InPlaceOff();
  }
  template <class TImage>
  void 
  ThresholdImageFilter2<TImage>::PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
      os << indent << "Median: "<< static_cast<typename NumericTraits<PixelType>::PrintType>(m_Median)<< std::endl;
      os << indent << "OutsideValue: "<< static_cast<typename NumericTraits<PixelType>::PrintType>(m_OutsideValue)<< std::endl;
      os << indent << "Lower: "<< static_cast<typename NumericTraits<PixelType>::PrintType>(m_Lower)<< std::endl;
      os << indent << "Upper: "<< static_cast<typename NumericTraits<PixelType>::PrintType>(m_Upper)<< std::endl;
      os << indent << "Select: "<< static_cast<typename NumericTraits<PixelType>::PrintType>(m_Select)<< std::endl;
      os << indent << "InsideValue: "<< static_cast<typename NumericTraits<PixelType>::PrintType>(m_InsideValue)<< std::endl;
    }
    template <class TImage>
    void 
    ThresholdImageFilter2<TImage>::ThresholdMedian(const PixelType &thresh)
    {
		m_Median=thresh;
		this->Modified();
	}
	template <class TImage>
	void 
	ThresholdImageFilter2<TImage>::ThresholdSelect(const PixelType &thresh)
	{
		m_Select=thresh;
		this->Modified();
	}
	template <class TImage>
	void
	ThresholdImageFilter2<TImage>::ThresholdInsideValue(const PixelType &thresh)
	{
		m_InsideValue=thresh;
		this->Modified();
	}
	template <class TImage>
    void
    ThresholdImageFilter2<TImage>::ThresholdAbove(const PixelType &thresh)
    {
		if (m_Upper != thresh|| m_Lower > NumericTraits<PixelType>::NonpositiveMin())
		{
			m_Lower = NumericTraits<PixelType>::NonpositiveMin();
			m_Upper = thresh;
			this->Modified();
		}
	}
	template <class TImage>
	void
	ThresholdImageFilter2<TImage>::ThresholdBelow(const PixelType &thresh)
	{
		if (m_Lower != thresh || m_Upper < NumericTraits<PixelType>::max())
		{
			m_Lower = thresh;
			m_Upper = NumericTraits<PixelType>::max();
			this->Modified
		}	
	}
	template <class TImage>
	void 
	ThresholdImageFilter2<TImage>::ThresholdOutside(const PixelType &lower, const PixelType &upper)
	{
		if (lower > upper)
		{
			itkExceptionMacro(<<"Lower threshold cannot be greater than upper threshold.");
			return;
		}
		if (m_Lower != lower || m_Upper != upper)
		{
			m_Lower = lower;
			m_Upper = upper;
			this->Modified();
		}
	}
	template <class TImage>
	void
	ThresholdImageFilter2<TImage>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,int threadId)
	{
		itkDebugMacro(<<"Actually executing");
		InputImagePointer inputPtr = this->GetInput();
		OutputImagePointer outputPtr = this->GetOutput(0);
		typedef ImageRegionConstIterator<TImage> InputIterator;
		typedef ImageRegionIterator<TImage> OutputIterator;
		InputIterator inIt(inputPtr, outputRegionForThread);
		OutputIterator outIt(outputPtr, outputRegionForThread);
		ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
		while( !outIt.IsAtEnd() )
		{
			const PixelType value = inIt.Get();
			if (m_Select==0)
			{
				if (value <= m_Median)
				{	
					outIt.Set(0);
				}
				else
				{
					outIt.Set(255);
				}
				++inlt;
				++outlt;
				progress.CompletedPiexl();
			}
			else if(m_Select==1)
			{
				if(m_Lower<=value&&value<=m_Upper)
				{
					outlt.Set(inlt.Get());
				}
				else
				{
					outlt.Set(m_OutsideValue);
				}
				++inlt;
				++outlt;
				progress.CompletedPixel();
			}
			else if(m_Select==2)
			{
				if(m_Lower<=value&&value<=m_Upper)
				{
					outlt.Set(m_insideValue);
				}
				else
				{	
					outlt.Set(inlt.Get());
				}
				++inlt;
				++outlt;
				progress.CompletedPixel();
			}
			else
			{
				outlt.Set(0);
				++inlt;
				++outlt;
				porgresss.ComplatedPixel();
			}
		}
	}
}
#endif
				
				
				
		
		
	
		
    
	
	
		
		
	
		
	
    
    
    
	
