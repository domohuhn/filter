#include "dh/filter.h"
#include "dh/butterworth.h"
#include "dh/chebyshev.h"
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

static DH_FILTER_RETURN_VALUE dh_create_moving_average(dh_filter_data* filter, dh_filter_parameters* options);
static DH_FILTER_RETURN_VALUE dh_create_moving_average_highpass(dh_filter_data* filter, dh_filter_parameters* options);
static DH_FILTER_RETURN_VALUE dh_iir_exponential_lowpass(dh_filter_data* filter, dh_filter_parameters* options);
static DH_FILTER_RETURN_VALUE dh_fir_exponential_lowpass(dh_filter_data* filter, dh_filter_parameters* options);
static DH_FILTER_RETURN_VALUE dh_iir_butterworth_high_lowpass(dh_filter_data* filter, dh_filter_parameters* options, bool lowpass);
static DH_FILTER_RETURN_VALUE dh_iir_butterworth_bandfilter(dh_filter_data* filter, dh_filter_parameters* options, bool bandpass);
static DH_FILTER_RETURN_VALUE dh_iir_chebyshev_high_lowpass(dh_filter_data* filter, dh_filter_parameters* options, bool lowpass);
static DH_FILTER_RETURN_VALUE dh_iir_chebyshev_bandfilter(dh_filter_data* filter, dh_filter_parameters* options, bool bandpass);
static DH_FILTER_RETURN_VALUE dh_fir_create_sinc(dh_filter_data* filter, dh_filter_parameters* options);
static DH_FILTER_RETURN_VALUE dh_fir_create_sinc_bandfilter(dh_filter_data* filter, dh_filter_parameters* options, bool bandpass);

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
            rv = dh_create_moving_average(filter, options);
            break;
        case DH_FIR_MOVING_AVERAGE_LOWPASS:
            rv = dh_create_moving_average(filter, options);
            break;
        case DH_FIR_MOVING_AVERAGE_HIGHPASS:
            rv = dh_create_moving_average_highpass(filter, options);
            break;
        case DH_FIR_EXPONENTIAL_MOVING_AVERAGE_LOWPASS:
            rv = dh_fir_exponential_lowpass(filter, options);
            break;
        case DH_FIR_BRICKWALL_LOWPASS: // falltrough
        case DH_FIR_BRICKWALL_HIGHPASS:
            rv = dh_fir_create_sinc(filter, options);
            break;
        case DH_FIR_BRICKWALL_BANDPASS: // falltrough
        case DH_FIR_BRICKWALL_BANDSTOP:
            rv = dh_fir_create_sinc_bandfilter(filter, options, options->filter_type == DH_FIR_BRICKWALL_BANDPASS);
            break;
        case DH_IIR_EXPONENTIAL_LOWPASS:
            rv = dh_iir_exponential_lowpass(filter, options);
            break;
        case DH_IIR_BUTTERWORTH_LOWPASS: // fallthrough
        case DH_IIR_BUTTERWORTH_HIGHPASS:
            rv = dh_iir_butterworth_high_lowpass(filter, options, options->filter_type == DH_IIR_BUTTERWORTH_LOWPASS);
            break;
        case DH_IIR_BUTTERWORTH_BANDPASS: // fallthrough
        case DH_IIR_BUTTERWORTH_BANDSTOP:
            rv = dh_iir_butterworth_bandfilter(filter, options, options->filter_type == DH_IIR_BUTTERWORTH_BANDPASS);
            break;
        case DH_IIR_CHEBYSHEV_LOWPASS: // fallthrough
        case DH_IIR_CHEBYSHEV_HIGHPASS:
            rv = dh_iir_chebyshev_high_lowpass(filter, options, options->filter_type == DH_IIR_CHEBYSHEV_LOWPASS);
            break;
        case DH_IIR_CHEBYSHEV_BANDPASS: // fallthrough
        case DH_IIR_CHEBYSHEV_BANDSTOP:
            rv = dh_iir_chebyshev_bandfilter(filter, options, options->filter_type == DH_IIR_CHEBYSHEV_BANDPASS);
            break;
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

static DH_FILTER_RETURN_VALUE dh_create_moving_average(dh_filter_data* filter, dh_filter_parameters* options)
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
    int xshift = count/2;
    for (size_t i=0; i<count; ++i) {
        int idx = i;
        double x = 2*M_PI*cutoff*(idx-xshift);
        data[i] = x!=0.0 ? sin(x)/x : 1.0;
    }
    double gain[1] = {1.0};
    
    double scale = cabs(1.0/dh_gain_at(data,count,gain,1,0.0));
    for(size_t i=0; i<count; ++i) {
        data[i] *= scale;
    }

    if (highpass) {
        for (size_t i=0; i<count; ++i) {
            if(i!=xshift) {
                data[i] = -data[i];
            } else {
                data[i] = 1.0 - data[i];
            }
        }
    }
}

static DH_FILTER_RETURN_VALUE dh_fir_create_sinc(dh_filter_data* filter, dh_filter_parameters* options)
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


static DH_FILTER_RETURN_VALUE dh_fir_create_sinc_bandfilter(dh_filter_data* filter, dh_filter_parameters* options, bool bandpass)
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
    assert(8 == (void*)(filter->buffer+ filter->buffer_length)-(void*)(coeff_in_temp_high + count_single_filter));

    fill_array_fir_sinc(coeff_in_temp_low,count_single_filter,cutoff_low, bandpass);
    fill_array_fir_sinc(coeff_in_temp_high,count_single_filter,cutoff_high, !bandpass);
    if(bandpass) {
        convolve_parameters(coeff_in_temp_low,coeff_in_temp_high,count_single_filter,filter->coefficients_in);
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


static DH_FILTER_RETURN_VALUE dh_create_moving_average_highpass(dh_filter_data* filter, dh_filter_parameters* options)
{
    if (dh_create_moving_average(filter, options) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    filter->coefficients_in[0] = 1.0 - filter->coefficients_in[0];
    for (size_t i=1; i<filter->number_coefficients_in; ++i) {
        filter->coefficients_in[i] = -filter->coefficients_in[i];
    }
    filter->initialized = true;
    return DH_FILTER_OK;
}


static DH_FILTER_RETURN_VALUE dh_iir_exponential_lowpass(dh_filter_data* filter, dh_filter_parameters* options)
{
    if (dh_filter_allocate_buffers(filter, 1, 2) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    double val = options->cutoff_frequency_low/options->sampling_frequency;
    filter->coefficients_in[0] = val;
    filter->coefficients_out[0] = 1.0;
    filter->coefficients_out[1] = -(1.0-val);
    return DH_FILTER_OK;
}

static DH_FILTER_RETURN_VALUE dh_fir_exponential_lowpass(dh_filter_data* filter, dh_filter_parameters* options)
{
    if (dh_filter_allocate_buffers(filter, options->filter_order, 0) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    double current = 1.0;
    double integrated = 0.0;
    // TODO Check if this is correct
    double val = options->cutoff_frequency_low/options->sampling_frequency;
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

static DH_FILTER_RETURN_VALUE dh_iir_butterworth_high_lowpass(dh_filter_data* filter, dh_filter_parameters* options, bool lowpass)
{
    size_t coefficients = options->filter_order + 1;
    if (dh_filter_allocate_buffers(filter, coefficients, coefficients) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    if (lowpass) {
        return compute_butterworth_lowpass_coefficients(filter->coefficients_in, filter->coefficients_out, options->filter_order,
            options->cutoff_frequency_low, options->sampling_frequency);
    } else {
        filter->initialized = true;
        return compute_butterworth_highpass_coefficients(filter->coefficients_in, filter->coefficients_out, options->filter_order,
            options->cutoff_frequency_low, options->sampling_frequency);
    }
}

static DH_FILTER_RETURN_VALUE dh_iir_butterworth_bandfilter(dh_filter_data* filter, dh_filter_parameters* options, bool bandpass)
{
    size_t coefficients = 2*options->filter_order + 1;
    if (dh_filter_allocate_buffers(filter, coefficients, coefficients) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    filter->initialized = true;
    return compute_butterworth_bandfilter_coefficients(filter->coefficients_in, filter->coefficients_out, options->filter_order,
        options->cutoff_frequency_low, options->cutoff_frequency_high,
        options->sampling_frequency,bandpass);
}


static DH_FILTER_RETURN_VALUE dh_iir_chebyshev_high_lowpass(dh_filter_data* filter, dh_filter_parameters* options, bool lowpass)
{
    size_t coefficients = options->filter_order + 1;
    if (dh_filter_allocate_buffers(filter, coefficients, coefficients) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    if (lowpass) {
        return compute_chebyshev_lowpass_coefficients(filter->coefficients_in, filter->coefficients_out, options->filter_order,
            options->cutoff_frequency_low, options->sampling_frequency,
            options->ripple);
    } else {
        filter->initialized = true;
        return compute_chebyshev_highpass_coefficients(filter->coefficients_in, filter->coefficients_out, options->filter_order,
            options->cutoff_frequency_low, options->sampling_frequency,
            options->ripple);
    }
}

static DH_FILTER_RETURN_VALUE dh_iir_chebyshev_bandfilter(dh_filter_data* filter, dh_filter_parameters* options, bool bandpass)
{
    size_t coefficients = 2*options->filter_order + 1;
    if (dh_filter_allocate_buffers(filter, coefficients, coefficients) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    filter->initialized = true;
    return compute_chebyshev_bandfilter_coefficients(filter->coefficients_in, filter->coefficients_out, options->filter_order,
        options->cutoff_frequency_low, options->cutoff_frequency_high,
        options->sampling_frequency,bandpass,options->ripple);
}

