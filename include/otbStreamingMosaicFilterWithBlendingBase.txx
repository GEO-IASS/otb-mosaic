#ifndef __StreamingMosaicFilterWithBlendingBase_txx
#define __StreamingMosaicFilterWithBlendingBase_txx

#include "otbStreamingMosaicFilterWithBlendingBase.h"

namespace otb {

template <class TInputImage, class TOutputImage, class TDistanceImage, class TInternalValueType>
StreamingMosaicFilterWithBlendingBase<TInputImage, TOutputImage, TDistanceImage, TInternalValueType>
::StreamingMosaicFilterWithBlendingBase()
{

  // Default distance offset
  m_DistanceOffset = 0.0;

  // Default interpolator
  typename DistanceImageDefaultInterpolatorType::Pointer interp = DistanceImageDefaultInterpolatorType::New();
  m_DistanceInterpolator = static_cast<DistanceImageInterpolatorType*>( interp.GetPointer() );

}

/*
 * This method is overrided because one index on two is not an input image, but
 * an input distance image
 */
template <class TInputImage, class TOutputImage, class TDistanceImage, class TInternalValueType>
void
StreamingMosaicFilterWithBlendingBase<TInputImage, TOutputImage, TDistanceImage, TInternalValueType>
::AddUsedInputImageIndex(unsigned int inputImageIndex)
{
  // Add this index only if input is an inputImage (i.e. not a distanceImage)
  if (inputImageIndex % 2 == 0)
    {
    itkDebugMacro(<<"Input #" << inputImageIndex << " is an InputImage and will be used");
    Superclass::AddUsedInputImageIndex(inputImageIndex);
    }
  else
    {
    itkDebugMacro(<<"Input #" << inputImageIndex << " is an DistanceImage and will NOT be used");
    }

}

/*
 * This method is overrided because there is twice less input images
 *  (1 input image on 2 is a DistanceImage)
 */
template <class TInputImage, class TOutputImage, class TDistanceImage, class TInternalValueType>
unsigned int
StreamingMosaicFilterWithBlendingBase<TInputImage, TOutputImage, TDistanceImage, TInternalValueType>
::GetNumberOfInputImages()
{
  return 0.5 * this->GetNumberOfInputs();
}

/*
 * Prepare:
 * 1 image pointer
 * 1 image interpolator
 * 1 image region, which correspond to the buffered region which can be used by the interpolator
 */
template <class TInputImage, class TOutputImage, class TDistanceImage, class TInternalValueType>
void
StreamingMosaicFilterWithBlendingBase<TInputImage, TOutputImage, TDistanceImage, TInternalValueType>
::PrepareDistanceImageAccessors(DistanceImageType ** currentDistanceImage,
                                DistanceImageInterpolatorPointer * distanceInterpolator){

  // Get number of used input images
  const unsigned int n = Superclass::GetNumberOfUsedInputImages();

  // Loop on input images
  for (unsigned int i = 0 ; i < n ; i++)
    {
    // Input distance image i
    currentDistanceImage[i] = static_cast<DistanceImageType *>(
        Superclass::ProcessObject::GetInput(Superclass::GetUsedInputImageIndice(i)+1) );

    // Distance interpolator i
    distanceInterpolator[i] = static_cast<DistanceImageInterpolatorType*>(
        (m_DistanceInterpolator->CreateAnother() ).GetPointer() );
    distanceInterpolator[i]->SetInputImage(currentDistanceImage[i]);
    }
}

} // end namespace otb

#endif
