#ifndef GENERATED_C_CODE_INCLUDED
#define GENERATED_C_CODE_INCLUDED
    
#ifdef __cplusplus
extern "C" {
#endif
    
#include "stddef.h"
    
// Generates a filter designed on the webpage of the
// dh filter library.
// This code was generated from your inputs and is your property.
// USE THIS CODE AT YOUR OWN RISK! THERE IS NO GUARANTEE THAT IT WILL WORK!
//
// See:
// https://github.com/domohuhn/filter
// Filter parameters:
// Filter type        : DH_IIR_BUTTERWORTH_LOWPASS
// Order              : 5
// Sampling Frequency : 100
// Cutoff (low)       : 15
// Cutoff (high)      : 35
// Ripple (db)        : -3
// Gain               : 1.0

// Data struct holding the past inputs and outputs. Initialize the struct by setting all values to 0.
typedef struct {
    double inputs[6];
    double outputs[6];
    size_t input_index;
    size_t output_index;
} filter_data;

// Updates the filter and returns the filtered value
inline double update_filter(filter_data* filter, double input) {
    filter->input_index = filter->input_index > 0 ? filter->input_index - 1: 5;
    filter->inputs[filter->input_index] = input;
    const double input_coefficients[]={0.0069331961301426015,
        0.03466598065071301,
        0.06933196130142602,
        0.06933196130142602,
        0.03466598065071301,
        0.0069331961301426015};
    double out = input * input_coefficients[0];
    size_t split_loops = 6 - filter->input_index;
    for(size_t i=1; i < split_loops; ++i) {
        out += input_coefficients[i] * filter->inputs[filter->input_index + i];
    }
    for(size_t i=split_loops; i < 6; ++i) {
        out += input_coefficients[i] * filter->inputs[i-split_loops];
    }
    const double output_coefficients[]={1,
        -1.9759016164414662,
        2.0134730260003075,
        -1.1026179777777696,
        0.3276183340001567,
        -0.04070948961666521};
    filter->output_index = filter->output_index > 0 ? filter->output_index - 1: 5;
    split_loops = 6 - filter->output_index;
    for(size_t i=1; i < split_loops; ++i) {
        out -= output_coefficients[i] * filter->outputs[filter->output_index + i];
    }
    for(size_t i=split_loops; i < 6; ++i) {
        out -= output_coefficients[i] * filter->outputs[i-split_loops];
    }
    filter->outputs[filter->output_index] = out;
    return out;
}

#ifdef __cplusplus
}
#endif

#endif /* GENERATED_C_CODE_INCLUDED */
