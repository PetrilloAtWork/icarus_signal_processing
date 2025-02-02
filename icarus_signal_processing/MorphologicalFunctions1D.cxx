#ifndef __SIGPROC_TOOLS_MORPHOLOGICALFUNCTIONS1D_CXX__
#define __SIGPROC_TOOLS_MORPHOLOGICALFUNCTIONS1D_CXX__

#include "MorphologicalFunctions1D.h"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cmath>
#include <functional>


namespace icarus_signal_processing 
{

void Dilation1D::operator()(const Waveform<short>& waveformVec,  
                            Waveform<short>&       dilationVec)  const
{
    getDilation<short>(waveformVec, dilationVec);
}

void Dilation1D::operator()(const Waveform<float>& waveformVec,  
                            Waveform<float>&       dilationVec)  const
{
    getDilation<float>(waveformVec, dilationVec);
}

void Dilation1D::operator()(const Waveform<double>& waveformVec,  
                            Waveform<double>&       dilationVec)  const
{
    getDilation<double>(waveformVec, dilationVec);
}


template <typename T> void icarus_signal_processing::Dilation1D::getDilation(const Waveform<T>& inputWaveform,
                                                                             Waveform<T>&       dilationVec) const
{
    /*
      Module for 1D Dilation Filter.

      INPUTS:
      - waveform: 1D Pedestal Corrected Waveform.
      - structuringElement: Size of moving window
  
      MODIFIES:
      - dilationVec: Returned Dilation Vector.
    */
/*
    // Set the window size
    int halfWindowSize(fStructuringElement/2);

    // Initialize min and max elements
    std::pair<typename Waveform<T>::const_iterator, typename Waveform<T>::const_iterator> minMaxItr = std::minmax_element(inputWaveform.begin(),inputWaveform.begin()+halfWindowSize);

    typename Waveform<T>::const_iterator minElementItr = minMaxItr.first;
    typename Waveform<T>::const_iterator maxElementItr = minMaxItr.second;

    // Initialize the erosion and dilation vectors
    dilationVec.resize(inputWaveform.size());

    // Now loop through remaining elements and complete the vectors
    typename Waveform<T>::iterator maxItr = dilationVec.begin();

    for (typename Waveform<T>::const_iterator inputItr = inputWaveform.begin(); inputItr != inputWaveform.end(); inputItr++)
    {
        // There are two conditions to check:
        // 1) is the current min/max element outside the current window?
        // 2) is the new element smaller/larger than the current min/max?
        // Make sure we are not running off the end of the vector
        if (std::distance(inputItr,inputWaveform.end()) > halfWindowSize)
        {
            if (std::distance(minElementItr,inputItr) >= halfWindowSize)
                minElementItr = std::min_element(inputItr - halfWindowSize, inputItr + halfWindowSize + 1);
            else if (*(inputItr + halfWindowSize) < *minElementItr)
                minElementItr = inputItr + halfWindowSize;
            if (std::distance(maxElementItr,inputItr) >= halfWindowSize)
                maxElementItr = std::max_element(inputItr - halfWindowSize, inputItr + halfWindowSize + 1);
            else if (*(inputItr + halfWindowSize) > *maxElementItr)
                maxElementItr = inputItr + halfWindowSize;
        }
        // Update the vectors
        *maxItr++ = *maxElementItr;
    }
*/

    size_t N = inputWaveform.size();

    if (N % fStructuringElement != 0) {
        std::cout << "Structuring element size must divide the input array size. Returning" << std::endl;
        return;
    }

    std::vector<T> suffixArr(N);
    std::vector<T> prefixArr(N);
//    dilationVec.resize(N);

    size_t se = (size_t) fStructuringElement;

    for (size_t i=0; i<N; ++i) 
    {
        if (i % se == 0) prefixArr[i] = inputWaveform[i];
        else             prefixArr[i] = std::max(prefixArr[i-1], inputWaveform[i]);
    }

    for (size_t i=N; i!=0; --i) 
    {
        if (i % se == 0) suffixArr[i-1] = inputWaveform[i-1];
        else             suffixArr[i-1] = std::max(suffixArr[i], inputWaveform[i-1]);
    }

    int prefixIndex = 0;
    int suffixIndex = 0;

    for (size_t i=0; i<se/2+1; ++i)    dilationVec[i] = prefixArr[i+se/2];

    for (size_t i=N; i >= N-se/2; --i) dilationVec[i-1] = suffixArr[i-1-se/2];

    for (size_t i=se/2+1; i<N-se/2; ++i) 
    {
        prefixIndex    = i + (se - 1) / 2;
        suffixIndex    = i - (se - 1) / 2;
        dilationVec[i] = std::max(prefixArr[prefixIndex],suffixArr[suffixIndex]);
    }

    return;
}

void Erosion1D::operator()(const Waveform<short>& waveformVec,  
                           Waveform<short>&       erosionVec)  const
{
    getErosion<short>(waveformVec, erosionVec);
}

void Erosion1D::operator()(const Waveform<float>& waveformVec,  
                           Waveform<float>&       erosionVec)  const
{
    getErosion<float>(waveformVec, erosionVec);
}

void Erosion1D::operator()(const Waveform<double>& waveformVec,  
                           Waveform<double>&       erosionVec)  const
{
    getErosion<double>(waveformVec, erosionVec);
}


template <typename T> void icarus_signal_processing::Erosion1D::getErosion(const Waveform<T>& inputWaveform,
                                                                           Waveform<T>&       erosionVec) const
{

    size_t N = inputWaveform.size();

    if (N % fStructuringElement != 0) {
      std::cout << "Structuring element size must divide the input array size. Returning" << std::endl;
      return;
    }

    std::vector<T> suffixArr(N);
    std::vector<T> prefixArr(N);
//    erosionVec.resize(N);

    size_t se = (size_t) fStructuringElement;

    for (size_t i=0; i<N; ++i) 
    {
        if (i % se == 0) prefixArr[i] = inputWaveform[i];
        else             prefixArr[i] = std::min(prefixArr[i-1], inputWaveform[i]);
    }

    for (size_t i=N; i!=0; --i) 
    {
        if (i % se == 0) suffixArr[i-1] = inputWaveform[i-1];
        else             suffixArr[i-1] = std::min(suffixArr[i], inputWaveform[i-1]);
    }

    int prefixIndex = 0;
    int suffixIndex = 0;

    // Boundary Cases
    for (size_t i=0; i<se/2+1; ++i)    erosionVec[i] = prefixArr[i+se/2];

    // Boundary Cases
    for (size_t i=N; i >= N-se/2; --i) erosionVec[i-1] = suffixArr[i-1-se/2];

    // Main Loop
    for (size_t i=se/2+1; i<N-se/2; ++i) 
    {
        prefixIndex = i + (se - 1) / 2;
        suffixIndex = i - (se - 1) / 2;
        erosionVec[i] = std::min(prefixArr[prefixIndex],suffixArr[suffixIndex]);
    }

/*
  size_t N = inputWaveform.size();
  size_t k = (size_t) structuringElement;
  if (N <= k) {
    std::cout << "Input array size " << N << " must be greater than structuring element size " << k << std::endl;
    return;
  }
  size_t bufferSize = N + 2 * (k/2) + (k - (N % k));
  size_t windowSize = k/2;
  size_t paddingSize = k - (N % k);
  std::vector<T> suffixArr(bufferSize);
  std::vector<T> prefixArr(bufferSize);
  dilationVec.resize(N);

  // Padding Operations on Buffers
  for (size_t i=0; i<windowSize; ++i) {
    suffixArr[i] = std::numeric_limits<T>::min();
    prefixArr[i] = std::numeric_limits<T>::min();
  }

  for (size_t i=N+windowSize; i<bufferSize; ++i) {
    suffixArr[i] = std::numeric_limits<T>::min();
    prefixArr[i] = std::numeric_limits<T>::min();
  }

  // Compute Prefix and Suffix Buffers
  for (size_t i=0; i<N+paddingSize; ++i) {
    if (i % k == 0) {
      prefixArr[i+windowSize] = inputWaveform[i];
    } else {
      prefixArr[i+windowSize] = std::max(prefixArr[i+windowSize-1], inputWaveform[i]);
    }
  }

  for (size_t i=N+paddingSize; i!=0; --i) {
    if (i > N) {
      // Compensate for divisibility padding (must be -inf)
      continue;
    }
    else if (i % k == 0) {
      suffixArr[i+windowSize-1] = inputWaveform[i-1];
    } 
    else {
      suffixArr[i+windowSize-1] = std::max(suffixArr[i+windowSize], inputWaveform[i-1]);
    }
  }

  int prefixIndex = 0;
  int suffixIndex = 0;

  for (size_t i=0; i<N; ++i) {
    prefixIndex = i + 2 * windowSize;
    suffixIndex = i;
    dilationVec[i] = std::max(prefixArr[prefixIndex],
      suffixArr[suffixIndex]);
  }
*/

    return;
}

void Gradient1D::operator()(const Waveform<short>& waveformVec,  
                            Waveform<short>&       gradientVec)  const
{
    getGradient<short>(waveformVec, gradientVec);
}

void Gradient1D::operator()(const Waveform<float>& waveformVec,  
                            Waveform<float>&       gradientVec)  const
{
    getGradient<float>(waveformVec, gradientVec);
}

void Gradient1D::operator()(const Waveform<double>& waveformVec,  
                            Waveform<double>&       gradientVec)  const
{
    getGradient<double>(waveformVec, gradientVec);
}

template <typename T> void icarus_signal_processing::Gradient1D::getGradient(const Waveform<T>& inputWaveform,
                                                                             Waveform<T>&       gradientVec) const
{
    // Set the window size
    int halfWindowSize(fStructuringElement/2);

    // Initialize min and max elements
    std::pair<typename Waveform<T>::const_iterator, typename Waveform<T>::const_iterator> minMaxItr = std::minmax_element(inputWaveform.begin(),inputWaveform.begin()+halfWindowSize);
  
    typename Waveform<T>::const_iterator minElementItr = minMaxItr.first;
    typename Waveform<T>::const_iterator maxElementItr = minMaxItr.second;
  
    // Initialize the erosion and dilation vectors
    gradientVec.resize(inputWaveform.size());
    // Now loop through remaining elements and complete the vectors
    typename Waveform<T>::iterator difItr = gradientVec.begin();
  
    for (typename Waveform<T>::const_iterator inputItr = inputWaveform.begin(); inputItr != inputWaveform.end(); inputItr++)
    {
        // There are two conditions to check:
        // 1) is the current min/max element outside the current window?
        // 2) is the new element smaller/larger than the current min/max?
        // Make sure we are not running off the end of the vector
        if (std::distance(inputItr,inputWaveform.end()) > halfWindowSize)
        {
            if (std::distance(minElementItr,inputItr) >= halfWindowSize)
                minElementItr = std::min_element(inputItr - halfWindowSize, inputItr + halfWindowSize + 1);
            else if (*(inputItr + halfWindowSize) < *minElementItr)
                minElementItr = inputItr + halfWindowSize;
            if (std::distance(maxElementItr,inputItr) >= halfWindowSize)
                maxElementItr = std::max_element(inputItr - halfWindowSize, inputItr + halfWindowSize + 1);
            else if (*(inputItr + halfWindowSize) > *maxElementItr)
                maxElementItr = inputItr + halfWindowSize;
        }
        // Update the vectors
        *difItr++ = *maxElementItr - *minElementItr;
    }
    return;
}

void Average1D::operator()(const Waveform<short>& waveformVec,  
                           Waveform<short>&       averageVec)  const
{
    getAverage<short>(waveformVec, averageVec);
}

void Average1D::operator()(const Waveform<float>& waveformVec,  
                           Waveform<float>&       averageVec)  const
{
    getAverage<float>(waveformVec, averageVec);
}

void Average1D::operator()(const Waveform<double>& waveformVec,  
                           Waveform<double>&       averageVec)  const
{
    getAverage<double>(waveformVec, averageVec);
}

template <typename T> void icarus_signal_processing::Average1D::getAverage(const Waveform<T>& inputWaveform,
                                                                           Waveform<T>&       averageVec) const
{
    // Set the window size
    int halfWindowSize(fStructuringElement/2);
    // Initialize min and max elements
    std::pair<typename Waveform<T>::const_iterator, typename Waveform<T>::const_iterator> minMaxItr = std::minmax_element(inputWaveform.begin(),inputWaveform.begin()+halfWindowSize);
  
    typename Waveform<T>::const_iterator minElementItr = minMaxItr.first;
    typename Waveform<T>::const_iterator maxElementItr = minMaxItr.second;
  
    // Initialize the erosion and dilation vectors
    averageVec.resize(inputWaveform.size());

    // Now loop through remaining elements and complete the vectors
    typename Waveform<T>::iterator avgItr = averageVec.begin();
  
    for (typename Waveform<T>::const_iterator inputItr = inputWaveform.begin(); inputItr != inputWaveform.end(); inputItr++)
    {
        // There are two conditions to check:
        // 1) is the current min/max element outside the current window?
        // 2) is the new element smaller/larger than the current min/max?
        // Make sure we are not running off the end of the vector
        if (std::distance(inputItr,inputWaveform.end()) > halfWindowSize)
        {
            if (std::distance(minElementItr,inputItr) >= halfWindowSize)
                minElementItr = std::min_element(inputItr - halfWindowSize, inputItr + halfWindowSize + 1);
            else if (*(inputItr + halfWindowSize) < *minElementItr)
                minElementItr = inputItr + halfWindowSize;
            if (std::distance(maxElementItr,inputItr) >= halfWindowSize)
                maxElementItr = std::max_element(inputItr - halfWindowSize, inputItr + halfWindowSize + 1);
            else if (*(inputItr + halfWindowSize) > *maxElementItr)
                maxElementItr = inputItr + halfWindowSize;
        }
        // Update the vectors
        *avgItr++ = 0.5 * (*maxElementItr + *minElementItr);
    }
    return;
}

void Median1D::operator()(const Waveform<short>& waveformVec,  
                          Waveform<short>&       medianVec)  const
{
    getMedian<short>(waveformVec, medianVec);
}

void Median1D::operator()(const Waveform<float>& waveformVec,  
                          Waveform<float>&       medianVec)  const
{
    getMedian<float>(waveformVec, medianVec);
}

void Median1D::operator()(const Waveform<double>& waveformVec,  
                          Waveform<double>&       medianVec)  const
{
    getMedian<double>(waveformVec, medianVec);
}

template <typename T> void icarus_signal_processing::Median1D::getMedian(const Waveform<T>& inputWaveform,
                                                                         Waveform<T>& medianVec) const
{
    // Set the window size
    int halfWindowSize(fStructuringElement/2);
    medianVec.resize(inputWaveform.size());
    size_t nTicks = inputWaveform.size();
  
    T median = 0;
  
    for (size_t i=0; i<nTicks; ++i) 
    {
        typename std::vector<T> localVec;
        localVec.reserve(fStructuringElement);
        int lbx = i - (int) halfWindowSize;
        int ubx = i + (int) halfWindowSize;
        size_t lowerBoundx = std::max(lbx, 0);
        size_t upperBoundx = std::min(ubx, (int) nTicks);
        for (size_t j=lowerBoundx; j<upperBoundx+1; ++j) {
            localVec.push_back(inputWaveform[j]);
        }
        if (fStructuringElement % 2 == 0) 
        {
            const auto m1 = localVec.begin() + localVec.size() / 2 - 1;
            const auto m2 = localVec.begin() + localVec.size() / 2;
            std::nth_element(localVec.begin(), m1, localVec.end());
            std::nth_element(localVec.begin(), m2, localVec.end());
            median = (*m1 + *m2) / 2.0;
        } else {
            median = localVec[localVec.size() / 2];
        }
        medianVec[i] = median;
    }
    return;
}



}

/*


void icarus_signal_processing::Morph1D::getOpeningAndClosing(
  const Waveform<short>& inputWaveform,
  const unsigned int structuringElement,
  Waveform<short>& openingVec,
  Waveform<short>& closingVec) const
{
  getOpeningAndClosing<short>(
    inputWaveform, structuringElement, openingVec, closingVec);
  return;
}

void icarus_signal_processing::Morph1D::getOpeningAndClosing(
  const Waveform<float>& inputWaveform,
  const unsigned int structuringElement,
  Waveform<float>& openingVec,
  Waveform<float>& closingVec) const
{
  getOpeningAndClosing<float>(
    inputWaveform, structuringElement, openingVec, closingVec);
  return;
}

void icarus_signal_processing::Morph1D::getOpeningAndClosing(
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
void icarus_signal_processing::Morph1D::getOpeningAndClosing(
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
