#ifndef __itkThresholdImageFilter2_h
#define __itkThresholdImageFilter2_h
#include "itkInPlaceImageFilter.h"
#include "itkConceptChecking.h"
namespace itk
{
	template <class TImage>
	class ITK_EXPORT ThresholdImageFilter2 : public InPlaceImageFilter<TImage,TImage>
	{
		public:
			typedef ThresholdImageFilter2 Self;
			typedef InPlaceImageFilter<TImage,TImage> Superclass;
			typedef SmartPointer<Self> Pointer;
			typedef SmartPointer<const Self> ConstPointer;
			itkNewMacro(Self); 
			itkTypeMacro(ThresholdImageFilter2, InPlaceImageFilter);
			typedef typename TImage::PixelType PixelType;
#ifdef ITK_USE_CONCEPT_CHECKING
			itkConceptMacro(PixelTypeComparableCheck,
				(Concept::Comparable<PixelType>));
			itkConceptMacro(PixelTypeOStreamWritableCheck,
				(Concept::OStreamWritable<PixelType>));
#endif
			itkSetMacro(Median,PixelType);
			itkGetConstMacro(Median,PixelType);
			void ThresholdMedian(const PixelType &thresh);
				itkSetMacro(Select,PixelType);
				itkGetConstMacro(Select,PixelType);
			void ThresholdSelect(const PixelType &thresh);
				itkSetMacro(InsideValue,PixelType);
				itkGetConstMacro(InsideValue,PixelType);
			void ThresholdInsideValue(const PixelType &thresh);
				itkSetMacro(OutsideValue,PixelType);
				itkGetConstMacro(OutsideValue,PixelType);
			void ThresholdAbove(const PixelType &thresh);
			void ThresholdBelow(const PixelType &thresh);
			void ThresholdOutside(const PixelType &lower, const PixelType &upper);
		        itkSetMacro(Lower, PixelType);
				itkGetConstMacro(Lower, PixelType);
				itkSetMacro(Upper, PixelType);
				itkGetConstMacro(Upper, PixelType);
			typedef TImage InputImageType;
			typedef typename InputImageType::ConstPointer InputImagePointer;
			typedef typename InputImageType::RegionType InputImageRegionType;
			typedef typename InputImageType::PixelType InputImagePixelType;
			typedef TImage OutputImageType;
			typedef typename OutputImageType::Pointer OutputImagePointer;
			typedef typename OutputImageType::RegionType OutputImageRegionType;
			typedef typename OutputImageType::PixelType OutputImagePixelType;
		protected:
			ThresholdImageFilter2();
			~ThresholdImageFilter2() {};
			void PrintSelf(std::ostream& os, Indent indent) const;
			void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,int threadId );
        private:
			ThresholdImageFilter2(const Self&); //purposely not implemented
			void operator=(const Self&); //purposely not implemented
			PixelType m_Median;
			PixelType m_OutsideValue;
			PixelType m_Lower;
			PixelType m_Upper;
			PixelType m_Select;
			PixelType m_InsideValue;
	};
} // end namespace itk
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkThresholdImageFilter2.txx"
#endif
#endif






