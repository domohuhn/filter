#include "dh/filter.h"
#include "dh/butterworth.h"
#include "dh/chebyshev.h"
#include "dh/utility.h"
#include <assert.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include "math.h"
#include "complex.h"

/**
 * @file 
 * @brief This file contains the code to initialize a filter.
 * 
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

static DH_FILTER_RETURN_VALUE create_moving_average(dh_filter_data* filter, dh_filter_parameters* options);
static DH_FILTER_RETURN_VALUE create_moving_average_highpass(dh_filter_data* filter, dh_filter_parameters* options);
static DH_FILTER_RETURN_VALUE iir_exponential_lowpass(dh_filter_data* filter, dh_filter_parameters* options);
static DH_FILTER_RETURN_VALUE fir_exponential_lowpass(dh_filter_data* filter, dh_filter_parameters* options);
static DH_FILTER_RETURN_VALUE iir_butterworth(dh_filter_data* filter, dh_filter_parameters* options, DH_FILTER_CHARACTERISTIC type);
static DH_FILTER_RETURN_VALUE iir_chebyshev(dh_filter_data* filter, dh_filter_parameters* options, DH_FILTER_CHARACTERISTIC type, bool isType2);
static DH_FILTER_RETURN_VALUE fir_create_sinc(dh_filter_data* filter, dh_filter_parameters* options);
static DH_FILTER_RETURN_VALUE fir_create_sinc_bandfilter(dh_filter_data* filter, dh_filter_parameters* options, bool bandpass);

DH_FILTER_RETURN_VALUE dh_create_filter(dh_filter_data* filter, dh_filter_parameters* options)
{
    assert(filter != NULL);
    assert(options != NULL);
    if(filter == NULL || options == NULL) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }
    DH_FILTER_RETURN_VALUE rv = DH_FILTER_UNKNOWN_FILTER_TYPE;
    switch(options->filter_type){
        case DH_NO_FILTER:
            options->filter_order = 0;
            rv = create_moving_average(filter, options);
            break;
        case DH_FIR_MOVING_AVERAGE_LOWPASS:
            rv = create_moving_average(filter, options);
            break;
        case DH_FIR_MOVING_AVERAGE_HIGHPASS:
            rv = create_moving_average_highpass(filter, options);
            break;
        case DH_FIR_EXPONENTIAL_MOVING_AVERAGE_LOWPASS:
            rv = fir_exponential_lowpass(filter, options);
            break;
        case DH_FIR_BRICKWALL_LOWPASS: // falltrough
        case DH_FIR_BRICKWALL_HIGHPASS:
            rv = fir_create_sinc(filter, options);
            break;
        case DH_FIR_BRICKWALL_BANDPASS: // falltrough
        case DH_FIR_BRICKWALL_BANDSTOP:
            rv = fir_create_sinc_bandfilter(filter, options, options->filter_type == DH_FIR_BRICKWALL_BANDPASS);
            break;
        case DH_IIR_EXPONENTIAL_LOWPASS:
            rv = iir_exponential_lowpass(filter, options);
            break;
        case DH_IIR_BUTTERWORTH_LOWPASS : return iir_butterworth(filter, options, DH_LOWPASS);
        case DH_IIR_BUTTERWORTH_HIGHPASS: return iir_butterworth(filter, options, DH_HIGHPASS);
        case DH_IIR_BUTTERWORTH_BANDPASS: return iir_butterworth(filter, options, DH_BANDPASS);
        case DH_IIR_BUTTERWORTH_BANDSTOP: return iir_butterworth(filter, options, DH_BANDSTOP);
        case DH_IIR_CHEBYSHEV_LOWPASS   : return iir_chebyshev(filter, options, DH_LOWPASS,false);
        case DH_IIR_CHEBYSHEV_HIGHPASS  : return iir_chebyshev(filter, options, DH_HIGHPASS,false);
        case DH_IIR_CHEBYSHEV_BANDPASS  : return iir_chebyshev(filter, options, DH_BANDPASS,false);
        case DH_IIR_CHEBYSHEV_BANDSTOP  : return iir_chebyshev(filter, options, DH_BANDSTOP,false);
        case DH_IIR_CHEBYSHEV2_LOWPASS  : return iir_chebyshev(filter, options, DH_LOWPASS,true);
        case DH_IIR_CHEBYSHEV2_HIGHPASS : return iir_chebyshev(filter, options, DH_HIGHPASS,true);
        case DH_IIR_CHEBYSHEV2_BANDPASS : return iir_chebyshev(filter, options, DH_BANDPASS,true);
        case DH_IIR_CHEBYSHEV2_BANDSTOP : return iir_chebyshev(filter, options, DH_BANDSTOP,true);
    }
    return rv;
}

static void zero_inout_buffers(dh_filter_data* filter) {
    for(size_t i=0; i<filter->number_coefficients_in; ++i) {
        filter->inputs[i] = 0.0;
    }
    for(size_t i=0; i<filter->number_coefficients_out; ++i) {
        filter->outputs[i] = 0.0;
    }
}

static DH_FILTER_RETURN_VALUE dh_filter_allocate_buffers(dh_filter_data* filter, size_t num_inputs, size_t num_outputs)
{
    size_t total_num = 2 * (num_inputs + num_outputs);
    filter->buffer_length = total_num * sizeof(double);
    filter->buffer = malloc(filter->buffer_length);
    if(filter->buffer == NULL) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    filter->buffer_needs_cleanup = true;

    size_t offset = 0;
    double* ptr = (double*)filter->buffer;
    if (num_inputs>0) {
        filter->coefficients_in = ptr;
        offset += num_inputs;
        filter->inputs = ptr + offset;
        offset += num_inputs;
    }
    else {
        filter->coefficients_in = NULL;
        filter->inputs = NULL;
    }

    if (num_outputs>0) {
        filter->coefficients_out = ptr + offset;
        offset += num_outputs;
        filter->outputs = ptr + offset;
        offset += num_outputs;
    }
    else {
        filter->coefficients_out = NULL;
        filter->outputs = NULL;
    }

    filter->current_input_index = 0;
    filter->current_output_index = 0;
    filter->number_coefficients_in = num_inputs;
    filter->number_coefficients_out = num_outputs;
    filter->initialized = false;
    
    zero_inout_buffers(filter);
    return DH_FILTER_OK;
}

static DH_FILTER_RETURN_VALUE create_moving_average(dh_filter_data* filter, dh_filter_parameters* options)
{
    if (dh_filter_allocate_buffers(filter, options->filter_order+1, 1) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    double val = 1.0/(double)filter->number_coefficients_in;
    for (size_t i=0; i<filter->number_coefficients_in; ++i) {
        filter->coefficients_in[i] = val;
    }
    filter->coefficients_out[0] = 1.0;
    return DH_FILTER_OK;
}

static void fill_array_fir_sinc(double* data, size_t count,double cutoff, bool highpass) {
    assert(data != NULL);
    int xshift = (int)count/2;
    for (size_t i=0; i<count; ++i) {
        int idx = (int)i;
        double x = 2*M_PI*cutoff*(idx-xshift);
        data[i] = x!=0.0 ? sin(x)/x : 1.0;
    }
    double gain[1] = {1.0};
    
    dh_normalize_gain_at(data,count,gain,1,0.0);

    if (highpass) {
        for (size_t i=0; i<count; ++i) {
            if(i!=(size_t)xshift) {
                data[i] = -data[i];
            } else {
                data[i] = 1.0 - data[i];
            }
        }
    }
}

static DH_FILTER_RETURN_VALUE fir_create_sinc(dh_filter_data* filter, dh_filter_parameters* options)
{
    if (dh_filter_allocate_buffers(filter, options->filter_order+1, 1) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    double cutoff = options->cutoff_frequency_low/options->sampling_frequency;
    bool is_highpass = options->filter_type == DH_FIR_BRICKWALL_HIGHPASS;
    fill_array_fir_sinc(filter->coefficients_in,filter->number_coefficients_in,cutoff , is_highpass);
    filter->coefficients_out[0] = 1.0;
    filter->initialized = options->filter_type != DH_FIR_BRICKWALL_LOWPASS;
    return DH_FILTER_OK;
}


static DH_FILTER_RETURN_VALUE fir_create_sinc_bandfilter(dh_filter_data* filter, dh_filter_parameters* options, bool bandpass)
{
    size_t count_single_filter = options->filter_order+1;
    if (dh_filter_allocate_buffers(filter, 2*count_single_filter-1, 1) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    double cutoff_low = options->cutoff_frequency_low/options->sampling_frequency;
    double cutoff_high = options->cutoff_frequency_high/options->sampling_frequency;
    // in order to save allocations, we will use the input and output buffers for the temporary values
    // inputs has size 2*count_single_filter-1 followed by 2 doubles
    // we need 2*count_single_filter -> ok, there are 8 bytes of buffer left
    double* coeff_in_temp_low = filter->inputs;
    double* coeff_in_temp_high = filter->inputs + count_single_filter;
    assert(8 == (char*)(filter->buffer+ filter->buffer_length)-(char*)(coeff_in_temp_high + count_single_filter));

    fill_array_fir_sinc(coeff_in_temp_low,count_single_filter,cutoff_low, bandpass);
    fill_array_fir_sinc(coeff_in_temp_high,count_single_filter,cutoff_high, !bandpass);
    if(bandpass) {
        dh_convolve_parameters(coeff_in_temp_low,coeff_in_temp_high,count_single_filter,filter->coefficients_in);
    } else {
        for(size_t i=0; i<count_single_filter;++i) {
            filter->coefficients_in[i]= coeff_in_temp_low[i] + coeff_in_temp_high[i];
        }
        filter->number_coefficients_in = count_single_filter;
    }
    filter->coefficients_out[0] = 1.0;
    filter->initialized = true;
    zero_inout_buffers(filter);
    return DH_FILTER_OK;
}


static DH_FILTER_RETURN_VALUE create_moving_average_highpass(dh_filter_data* filter, dh_filter_parameters* options)
{
    if (create_moving_average(filter, options) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    filter->coefficients_in[0] = 1.0 - filter->coefficients_in[0];
    for (size_t i=1; i<filter->number_coefficients_in; ++i) {
        filter->coefficients_in[i] = -filter->coefficients_in[i];
    }
    filter->initialized = true;
    return DH_FILTER_OK;
}


static DH_FILTER_RETURN_VALUE iir_exponential_lowpass(dh_filter_data* filter, dh_filter_parameters* options)
{
    if (dh_filter_allocate_buffers(filter, 1, 2) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    double val = options->cutoff_frequency_low/options->sampling_frequency;
    filter->coefficients_in[0] = val;
    filter->coefficients_out[0] = 1.0;
    filter->coefficients_out[1] = -1.0+val;
    return DH_FILTER_OK;
}

static DH_FILTER_RETURN_VALUE fir_exponential_lowpass(dh_filter_data* filter, dh_filter_parameters* options)
{
    if (dh_filter_allocate_buffers(filter, options->filter_order+1, 1) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    double val = 1.0-(options->cutoff_frequency_low/options->sampling_frequency);
    double current = val;
    double integrated = 0.0;
    filter->coefficients_out[0] = 1.0;
    for (size_t i=0; i<filter->number_coefficients_in; ++i) {
        filter->coefficients_in[i] = current;
        integrated += current;
        current *= val;
    }
    for (size_t i=0; i<filter->number_coefficients_in; ++i) {
        filter->coefficients_in[i] = filter->coefficients_in[i]/integrated;
    }
    return DH_FILTER_OK;
}

static DH_FILTER_RETURN_VALUE iir_butterworth(dh_filter_data* filter, dh_filter_parameters* options, DH_FILTER_CHARACTERISTIC type)
{
    size_t coefficients = type==DH_LOWPASS || type==DH_HIGHPASS ? options->filter_order + 1 : 2*options->filter_order + 1;
    if (dh_filter_allocate_buffers(filter, coefficients, coefficients) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    filter->initialized = type!=DH_LOWPASS;
    return dh_compute_butterworth_filter_coefficients(filter,options,type);
}

static DH_FILTER_RETURN_VALUE iir_chebyshev(dh_filter_data* filter, dh_filter_parameters* options, DH_FILTER_CHARACTERISTIC type, bool isType2)
{
    size_t coefficients = type==DH_LOWPASS || type==DH_HIGHPASS ? options->filter_order + 1 : 2*options->filter_order + 1;
    if (dh_filter_allocate_buffers(filter, coefficients, coefficients) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    filter->initialized = type!=DH_LOWPASS;
    return dh_compute_chebyshev_filter_coefficients(filter,options,type,isType2);
}

