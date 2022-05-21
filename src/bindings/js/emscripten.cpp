#include "dh/filter-types.h"
#include "cpp/filter.hpp"
#include <cstdio>
#include <emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;



void Foo2(const dh_filter_parameters x) {
  printf("Got freq: %f\n",x.sampling_frequency);
  printf("Got low: %f\n",x.cutoff_frequency_low);
  printf("Got hi: %f\n",x.cutoff_frequency_high);
  printf("Got ripp: %f\n",x.ripple);
  printf("Got ord: %zu\n",x.filter_order);
  printf("Got type: %u\n",x.filter_type);
}

EMSCRIPTEN_BINDINGS(filter_params) {
  enum_<DH_FILTER_TYPE>("FilterType")
    .value("NO_FILTER", DH_NO_FILTER)
    .value("FIR_MOVING_AVERAGE_LOWPASS", DH_FIR_MOVING_AVERAGE_LOWPASS)
    .value("FIR_MOVING_AVERAGE_LOWPASS", DH_FIR_MOVING_AVERAGE_HIGHPASS)
    .value("FIR_EXPONENTIAL_MOVING_AVERAGE_LOWPASS", DH_FIR_EXPONENTIAL_MOVING_AVERAGE_LOWPASS)
    .value("FIR_BRICKWALL_LOWPASS", DH_FIR_BRICKWALL_LOWPASS)
    .value("FIR_BRICKWALL_HIGHPASS", DH_FIR_BRICKWALL_HIGHPASS)
    .value("FIR_BRICKWALL_BANDPASS", DH_FIR_BRICKWALL_BANDPASS)
    .value("FIR_BRICKWALL_BANDSTOP", DH_FIR_BRICKWALL_BANDSTOP)
    .value("IIR_EXPONENTIAL_LOWPASS", DH_IIR_EXPONENTIAL_LOWPASS)
    .value("IIR_BUTTERWORTH_LOWPASS", DH_IIR_BUTTERWORTH_LOWPASS)
    .value("IIR_BUTTERWORTH_HIGHPASS", DH_IIR_BUTTERWORTH_HIGHPASS)
    .value("IIR_BUTTERWORTH_BANDPASS", DH_IIR_BUTTERWORTH_BANDPASS)
    .value("IIR_BUTTERWORTH_BANDSTOP", DH_IIR_BUTTERWORTH_BANDSTOP)
    .value("IIR_CHEBYSHEV_LOWPASS", DH_IIR_CHEBYSHEV_LOWPASS)
    .value("IIR_CHEBYSHEV_HIGHPASS", DH_IIR_CHEBYSHEV_HIGHPASS)
    .value("IIR_CHEBYSHEV_BANDPASS", DH_IIR_CHEBYSHEV_BANDPASS)
    .value("IIR_CHEBYSHEV_BANDSTOP", DH_IIR_CHEBYSHEV_BANDSTOP);

  class_<dh_filter_parameters>("FilterParameters")
    .constructor<>()
    .property("cutoffFrequencyLow", &dh_filter_parameters::cutoff_frequency_low)
    .property("cutoffFrequencyHigh", &dh_filter_parameters::cutoff_frequency_high)
    .property("samplingFrequency", &dh_filter_parameters::sampling_frequency)
    .property("ripple", &dh_filter_parameters::ripple)
    .property("filterOrder", &dh_filter_parameters::filter_order)
    .property("filterType", &dh_filter_parameters::filter_type);
    
  function("Foo2", &Foo2);
}


size_t countFeedForwardCoefficients(const dh::filter& filter) {
  return filter.feedforward_coefficients().size();
}

size_t countFeedBackCoefficients(const dh::filter& filter) {
  return filter.feedback_coefficients().size();
}

val feedforwardCoefficient(const dh::filter& filter, size_t index) {
  auto coeff = filter.feedforward_coefficients();
  if (index < coeff.size()) {
    return val(coeff[index]);
  } else {
    return val::undefined();
  }
}

val feedbackCoefficient(const dh::filter& filter, size_t index) {
  auto coeff = filter.feedback_coefficients();
  if (index < coeff.size()) {
    return val(coeff[index]);
  } else {
    return val::undefined();
  }
}

class frequency_response {
public:
  frequency_response(std::vector<dh_frequency_response_t> d) : data_(d) {}

  size_t size() const noexcept {
    return data_.size();
  }

  val frequency(size_t i) const noexcept {
    if (i < data_.size()) {
      return val(data_[i].frequency);
    } else {
      return val::undefined();
    }
  }

  val gain(size_t i) const noexcept {
    if (i < data_.size()) {
      return val(data_[i].gain);
    } else {
      return val::undefined();
    }
  }

  val phase(size_t i) const noexcept {
    if (i < data_.size()) {
      return val(data_[i].phase_shift);
    } else {
      return val::undefined();
    }
  }

private:
  std::vector<dh_frequency_response_t> data_;
};

frequency_response frequencyResponse(const dh::filter& filter, size_t count) {
  return frequency_response(filter.compute_frequency_response(count));
}

EMSCRIPTEN_BINDINGS(filter_wrapper) {
  class_<frequency_response>("FrequencyResponse")
    .property("size", &frequency_response::size)
    .function("frequency", &frequency_response::frequency)
    .function("gain", &frequency_response::gain)
    .function("phase", &frequency_response::phase);
  
  class_<dh::filter>("Filter")
    .constructor<dh_filter_parameters>()
    .property("countFeedforwardCoefficients", &countFeedForwardCoefficients)
    .property("countFeedbackCoefficients", &countFeedBackCoefficients)
    .function("feedforwardCoefficient", &feedforwardCoefficient)
    .function("feedbackCoefficient", &feedbackCoefficient)
    .function("frequencyResponse", &frequencyResponse)
    ;
}

