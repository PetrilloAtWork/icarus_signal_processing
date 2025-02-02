/**
 * \file Deconvolution.h
 *
 * \ingroup icarus_signal_processing
 * 
 * \brief Class def header for a class Deconvolution
 *
 * @author koh0207
 */

/** \addtogroup icarus_signal_processing

    @{*/
#ifndef __icarus_signal_processing_DECONVOLUTION_H__

#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cmath>
#include <functional>
#include "MiscUtils.h"

#include <Eigen/Core>
#include <unsupported/Eigen/FFT>


#define __icarus_signal_processing_DECONVOLUTION_H__

namespace icarus_signal_processing {

  /**
     \class Deconvolution
     User defined class Deconvolution ... these comments are used to generate
     doxygen documentation!
  */
  class Deconvolution{
    
    public:
      
      /// Default constructor
      Deconvolution(){}

      void Inverse1D(
        std::vector<std::vector<float>>&,
        const std::vector<std::vector<float>>&,
        const std::vector<float>&
      );

      void Inverse1D(
        std::vector<std::vector<double>>&,
        const std::vector<std::vector<double>>&,
        const std::vector<double>&
      );


      void Wiener1D(
        std::vector<std::vector<float>>&,
        const std::vector<std::vector<float>>&,
        const std::vector<float>&,
        const float
      );

      void Wiener1D(
        std::vector<std::vector<double>>&,
        const std::vector<std::vector<double>>&,
        const std::vector<double>&,
        const float
      );


      void AdaptiveWiener1D(
        std::vector<std::vector<float>>&,
        const std::vector<std::vector<float>>&,
        const std::vector<float>&,
        const std::vector<std::vector<bool>>&
      );

      void AdaptiveWiener1D(
        std::vector<std::vector<double>>&,
        const std::vector<std::vector<double>>&,
        const std::vector<double>&,
        const std::vector<std::vector<bool>>&
      );

      void FourierShrinkage1D(
        std::vector<std::vector<float>>& outputWaveform,
        const std::vector<std::vector<float>>& inputWaveform,
        const std::vector<float>& responseFunction,
        const float regParam);

      
      /// Default destructor
      ~Deconvolution(){}

    private:

      template <typename T>
      void Inverse1D(
        std::vector<std::vector<T>>& outputWaveform,
        const std::vector<std::vector<T>>& inputWaveform,
        const std::vector<T>& responseFunction
      );

      template <typename T>
      void Wiener1D(
        std::vector<std::vector<T>>& outputWaveform,
        const std::vector<std::vector<T>>& inputWaveform,
        const std::vector<T>& responseFunction,
        const float noiseVar
      );

      template <typename T>
      void AdaptiveWiener1D(
        std::vector<std::vector<T>>& outputWaveform,
        const std::vector<std::vector<T>>& inputWaveform,
        const std::vector<T>& responseFunction,
        const std::vector<std::vector<bool>>& selectVals
      );
      
    };
}

#endif
/** @} */ // end of doxygen group 

