#ifndef __SIGPROC_TOOLS_MorphologicalFunctions2D_CXX__
#define __SIGPROC_TOOLS_MorphologicalFunctions2D_CXX__

#include "MorphologicalFunctions2D.h"
#include "MorphologicalFunctions1D.h"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cmath>
#include <functional>


namespace icarus_signal_processing 
{


void icarus_signal_processing::Dilation2D::operator()(ArrayFloat::const_iterator inputWaveform2D, 
                                                      unsigned int               numChannels, 
                                                      ArrayFloat::iterator       dilation2D) const
{
    /*
      Module for 2D Dilation Filter.

      INPUTS:
      - waveform: 2D Pedestal Corrected Waveform.
      - structuringElement: Size of moving window
  
      MODIFIES:
      - dilationVec: Returned Dilation Vector.
    */
    size_t nTicks = (*inputWaveform2D).size();

    for (size_t i=0; i<numChannels; ++i) dilation2D[i].resize(nTicks);

    icarus_signal_processing::Dilation1D dilation1DX(fStructuringElementX);
    icarus_signal_processing::Dilation1D dilation1DY(fStructuringElementY);

    for (size_t i=0; i<numChannels; ++i) dilation1DY(*(inputWaveform2D+i), dilation2D[i]);

    for (size_t j=0; j<nTicks; ++j) 
    {
        Waveform<float> column(numChannels);
        Waveform<float> columnOut(numChannels);
    
        for (size_t i=0; i<numChannels; ++i) column[i] = dilation2D[i][j];

        dilation1DX(column, columnOut);
    
        for (size_t i=0; i<numChannels; ++i) dilation2D[i][j] = columnOut[i];
    }

    return;
}

void icarus_signal_processing::Erosion2D::operator()(ArrayFloat::const_iterator inputWaveform2D,
                                                     unsigned int               numChannels, 
                                                     ArrayFloat::iterator       erosion2D) const
{
    size_t nTicks = (*inputWaveform2D).size();

    for (size_t i=0; i<numChannels; ++i) erosion2D[i].resize(nTicks);


    icarus_signal_processing::Erosion1D erosion1DX(fStructuringElementX);
    icarus_signal_processing::Erosion1D erosion1DY(fStructuringElementY);

    for (size_t i=0; i<numChannels; ++i) erosion1DY(*(inputWaveform2D+i), erosion2D[i]);

    for (size_t j=0; j<nTicks; ++j) {
        Waveform<float> column(numChannels);
        Waveform<float> columnOut(numChannels);

        for (size_t i=0; i<numChannels; ++i) column[i] = erosion2D[i][j];

        erosion1DX(column, columnOut);

        for (size_t i=0; i<numChannels; ++i) erosion2D[i][j] = columnOut[i];
    }

    return;
}

void icarus_signal_processing::Gradient2D::operator()(ArrayFloat::const_iterator inputWaveform2D,
                                                     unsigned int                numChannels, 
                                                     ArrayFloat::iterator        gradient2D) const
{
    size_t nTicks = (*inputWaveform2D).size();

    for (size_t i=0; i<numChannels; ++i) gradient2D[i].resize(nTicks);

    ArrayFloat dilation2D(numChannels);
    ArrayFloat erosion2D(numChannels);

    icarus_signal_processing::Dilation2D(fStructuringElementX, fStructuringElementY)(inputWaveform2D, numChannels, dilation2D.begin());
    icarus_signal_processing::Erosion2D (fStructuringElementX, fStructuringElementY)(inputWaveform2D, numChannels, erosion2D.begin());

    for (size_t i=0; i<numChannels; ++i) {
        for (size_t j=0; j<nTicks; ++j) {
            gradient2D[i][j] = (dilation2D[i][j] - erosion2D[i][j]);
        }
    }

    return;
}

void icarus_signal_processing::Average2D::operator()(ArrayFloat::const_iterator inputWaveform2D,
                                                     unsigned int                numChannels, 
                                                     ArrayFloat::iterator        averageVec) const
{
//    // Set the window size
//    int halfWindowSize(fStructuringElement/2);
//    // Initialize min and max elements
//    std::pair<typename Waveform<T>::const_iterator, typename Waveform<T>::const_iterator> minMaxItr = std::minmax_element(inputWaveform.begin(),inputWaveform.begin()+halfWindowSize);
//  
//    typename Waveform<T>::const_iterator minElementItr = minMaxItr.first;
//    typename Waveform<T>::const_iterator maxElementItr = minMaxItr.second;
//  
//    // Initialize the erosion and dilation vectors
//    averageVec.resize(inputWaveform.size());
//
//    // Now loop through remaining elements and complete the vectors
//    typename Waveform<T>::iterator avgItr = averageVec.begin();
//  
//    for (typename Waveform<T>::const_iterator inputItr = inputWaveform.begin(); inputItr != inputWaveform.end(); inputItr++)
//    {
//        // There are two conditions to check:
//        // 1) is the current min/max element outside the current window?
//        // 2) is the new element smaller/larger than the current min/max?
//        // Make sure we are not running off the end of the vector
//        if (std::distance(inputItr,inputWaveform.end()) > halfWindowSize)
//        {
//            if (std::distance(minElementItr,inputItr) >= halfWindowSize)
//                minElementItr = std::min_element(inputItr - halfWindowSize, inputItr + halfWindowSize + 1);
//            else if (*(inputItr + halfWindowSize) < *minElementItr)
//                minElementItr = inputItr + halfWindowSize;
//            if (std::distance(maxElementItr,inputItr) >= halfWindowSize)
//                maxElementItr = std::max_element(inputItr - halfWindowSize, inputItr + halfWindowSize + 1);
//            else if (*(inputItr + halfWindowSize) > *maxElementItr)
//                maxElementItr = inputItr + halfWindowSize;
//        }
//        // Update the vectors
//        *avgItr++ = 0.5 * (*maxElementItr + *minElementItr);
//    }
    return;
}

void icarus_signal_processing::Median2D::operator()(ArrayFloat::const_iterator inputWaveform2D,
                                                    unsigned int                numChannels, 
                                                    ArrayFloat::iterator        medianVec) const
{
//    // Set the window size
//    int halfWindowSize(fStructuringElement/2);
//    medianVec.resize(inputWaveform.size());
//    size_t nTicks = inputWaveform.size();
//  
//    T median = 0;
//  
//    for (size_t i=0; i<nTicks; ++i) 
//    {
//        typename std::vector<T> localVec;
//        localVec.reserve(fStructuringElement);
//        int lbx = i - (int) halfWindowSize;
//        int ubx = i + (int) halfWindowSize;
//        size_t lowerBoundx = std::max(lbx, 0);
//        size_t upperBoundx = std::min(ubx, (int) nTicks);
//        for (size_t j=lowerBoundx; j<upperBoundx+1; ++j) {
//            localVec.push_back(inputWaveform[j]);
//        }
//        if (fStructuringElement % 2 == 0) 
//        {
//            const auto m1 = localVec.begin() + localVec.size() / 2 - 1;
//            const auto m2 = localVec.begin() + localVec.size() / 2;
//            std::nth_element(localVec.begin(), m1, localVec.end());
//            std::nth_element(localVec.begin(), m2, localVec.end());
//            median = (*m1 + *m2) / 2.0;
//        } else {
//            median = localVec[localVec.size() / 2];
//        }
//        medianVec[i] = median;
//    }
    return;
}



}

/*


void icarus_signal_processing::Morph2D::getOpeningAndClosing(
  const Waveform<short>& inputWaveform,
  const unsigned int structuringElement,
  Waveform<short>& openingVec,
  Waveform<short>& closingVec) const
{
  getOpeningAndClosing<short>(
    inputWaveform, structuringElement, openingVec, closingVec);
  return;
}

void icarus_signal_processing::Morph2D::getOpeningAndClosing(
  const Waveform<float>& inputWaveform,
  const unsigned int structuringElement,
  Waveform<float>& openingVec,
  Waveform<float>& closingVec) const
{
  getOpeningAndClosing<float>(
    inputWaveform, structuringElement, openingVec, closingVec);
  return;
}

void icarus_signal_processing::Morph2D::getOpeningAndClosing(
  const Waveform<double>& inputWaveform,
  const unsigned int structuringElement,
  Waveform<double>& openingVec,
  Waveform<double>& closingVec) const
{
  getOpeningAndClosing<double>(
    inputWaveform, structuringElement, openingVec, closingVec);
  return;
}


template <typename T> 
void icarus_signal_processing::Morph2D::getOpeningAndClosing(
  const Waveform<T>& inputWaveform,
  const unsigned int structuringElement,
  Waveform<T>& openingVec,
  Waveform<T>& closingVec) const
{
  Waveform<T> dilationVec;
  Waveform<T> erosionVec;

  getDilation(inputWaveform, structuringElement, dilationVec);
  getErosion(inputWaveform, structuringElement, erosionVec);
  // Set the window size
  int halfWindowSize(structuringElement/2);
  // Start with the opening: get the max element in the input erosion vector
  typename Waveform<T>::iterator maxElementItr = 
    std::max_element(erosionVec.begin(),erosionVec.begin()+halfWindowSize);
  // Initialize the opening vector
  openingVec.resize(erosionVec.size());
  // Now loop through remaining elements and complete the vectors
  typename Waveform<T>::iterator maxItr = openingVec.begin();
  for (typename Waveform<T>::iterator inputItr = erosionVec.begin(); 
    inputItr != erosionVec.end(); inputItr++)
  {
    // There are two conditions to check:
    // 1) is the current min/max element outside the current window?
    // 2) is the new element smaller/larger than the current min/max?
    // Make sure we are not running off the end of the vector
    if (std::distance(inputItr,erosionVec.end()) > halfWindowSize)
    {
      if (std::distance(maxElementItr,inputItr) >= halfWindowSize)
          maxElementItr = std::max_element(
            inputItr - halfWindowSize, inputItr + halfWindowSize + 1);
      else if (*(inputItr + halfWindowSize) > *maxElementItr)
          maxElementItr = inputItr + halfWindowSize;
    }
    // Update the vectors
    *maxItr++ = *maxElementItr;
  }
  // Now go with the closing: get the min element in the input dilation vector
  typename Waveform<T>::iterator minElementItr = std::min_element(
    dilationVec.begin(),dilationVec.begin()+halfWindowSize);
  // Initialize the opening and closing vectors
  closingVec.resize(dilationVec.size());
  // Now loop through remaining elements and complete the vectors
  typename Waveform<T>::iterator minItr = closingVec.begin();
  for (typename Waveform<T>::iterator inputItr = dilationVec.begin(); 
    inputItr != dilationVec.end(); inputItr++)
  {
    // There are two conditions to check:
    // 1) is the current min/max element outside the current window?
    // 2) is the new element smaller/larger than the current min/max?
    // Make sure we are not running off the end of the vector
    if (std::distance(inputItr,dilationVec.end()) > halfWindowSize)
    {
      if (std::distance(minElementItr,inputItr) >= halfWindowSize)
          minElementItr = std::min_element(
            inputItr - halfWindowSize, inputItr + halfWindowSize + 1);
      else if (*(inputItr + halfWindowSize) < *minElementItr)
          minElementItr = inputItr + halfWindowSize;
    }
    // Update the vectors
    *minItr++ = *minElementItr;
  }
  return;
}
*/

#endif
