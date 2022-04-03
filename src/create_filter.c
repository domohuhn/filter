#include "dh/filter.h"
#include <assert.h>
#include <stdlib.h>

/**
 * @file 
 * @brief This file contains the code to initialize a filter.
 * 
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

static DH_FILTER_RETURN_VALUE dh_create_moving_average(dh_filter_data* filter, dh_filter_options* options);
static DH_FILTER_RETURN_VALUE dh_create_moving_average_highpass(dh_filter_data* filter, dh_filter_options* options);
static DH_FILTER_RETURN_VALUE dh_iir_exponential_lowpass(dh_filter_data* filter, dh_filter_options* options);
static DH_FILTER_RETURN_VALUE dh_iir_exponential_highpass(dh_filter_data* filter, dh_filter_options* options);

DH_FILTER_RETURN_VALUE dh_create_filter(dh_filter_data* filter, dh_filter_options* options)
{
    assert(filter != NULL);
    assert(options != NULL);
    if(filter == NULL || options == NULL) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }
    DH_FILTER_RETURN_VALUE rv = DH_FILTER_UNKNOWN_FILTER_TYPE;
    switch(options->filter_type){
        case DH_MOVING_AVERAGE:
            rv = dh_create_moving_average(filter, options);
            break;
        case DH_MOVING_AVERAGE_HIGHPASS:
            rv = dh_create_moving_average_highpass(filter, options);
            break;
        case DH_IIR_EXPONENTIAL_LOWPASS:
            rv = dh_iir_exponential_lowpass(filter, options);
            break;
    }
    return rv;
}

static DH_FILTER_RETURN_VALUE dh_filter_allocate_buffers(dh_filter_data* filter, size_t num_inputs, size_t num_outputs)
{
    size_t total_num = 2 * (num_inputs + num_outputs);
    filter->buffer_length = total_num * sizeof(DH_FILTER_VALUE_TYPE);
    filter->buffer = malloc(filter->buffer_length);
    if(filter->buffer == NULL) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    filter->buffer_needs_cleanup = true;

    size_t offset = 0;
    DH_FILTER_VALUE_TYPE* ptr = (DH_FILTER_VALUE_TYPE*)filter->buffer;
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

    
    for(size_t i=0; i<filter->number_coefficients_in; ++i) {
        filter->inputs[i] = 0.0;
    }
    for(size_t i=0; i<filter->number_coefficients_out; ++i) {
        filter->outputs[i] = 0.0;
    }
    return DH_FILTER_OK;
}

static DH_FILTER_RETURN_VALUE dh_create_moving_average(dh_filter_data* filter, dh_filter_options* options)
{
    if (dh_filter_allocate_buffers(filter, options->parameters.moving_average.filter_order, 0) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    DH_FILTER_VALUE_TYPE val = 1.0/(DH_FILTER_VALUE_TYPE)filter->number_coefficients_in;
    for (size_t i=0; i<filter->number_coefficients_in; ++i) {
        filter->coefficients_in[i] = val;
    }
    return DH_FILTER_OK;
}

static DH_FILTER_RETURN_VALUE dh_create_moving_average_highpass(dh_filter_data* filter, dh_filter_options* options)
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


static DH_FILTER_RETURN_VALUE dh_iir_exponential_lowpass(dh_filter_data* filter, dh_filter_options* options)
{
    if (dh_filter_allocate_buffers(filter, 1, 2) != DH_FILTER_OK) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    DH_FILTER_VALUE_TYPE val = options->parameters.exponential.alpha;
    filter->coefficients_in[0] = val;
    filter->coefficients_out[0] = 1.0;
    filter->coefficients_out[1] = -(1.0-val);
    return DH_FILTER_OK;
}




