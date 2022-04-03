#include "dh/filter.h"
#include <assert.h>
#include <stdio.h>

/**
 * @file 
 * @brief This file contains the code to run a filter cycle.
 * 
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

static void dh_filter_add_input(dh_filter_data* filter, DH_FILTER_VALUE_TYPE input);
static double dh_filter_run_filter_loop(DH_FILTER_VALUE_TYPE* coefficients, size_t num_coeffs, DH_FILTER_VALUE_TYPE* data, size_t current_idx , size_t start);
static void dh_filter_run_input_loop(dh_filter_data* filter);
static void dh_filter_run_output_loop(dh_filter_data* filter);
static void dh_filter_shift_output(dh_filter_data* filter);
static void dh_filter_add_output(dh_filter_data* filter);

DH_FILTER_RETURN_VALUE dh_filter(dh_filter_data* filter, DH_FILTER_VALUE_TYPE input, DH_FILTER_VALUE_TYPE* output)
{
    assert(filter);
    if (!filter) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }
    if (filter->number_coefficients_in == 0 || filter->inputs == NULL || filter->coefficients_in == NULL) {
        return DH_FILTER_DATA_STRUCTURE_INPUTS_NOT_INITIALIZED;
    }
    if(!filter->initialized) {
        // printf("==> init\n");
        dh_initialize_filter(filter,input);
    }

    dh_filter_add_input(filter, input);
    dh_filter_run_input_loop(filter);
    // printf("/// inputs %f current %f\n", input,filter->current_value);

    if (filter->number_coefficients_out > 0) {
        if (filter->outputs == NULL || filter->coefficients_out == NULL) {
            return DH_FILTER_DATA_STRUCTURE_OUTPUTS_NOT_INITIALIZED;
        }
        dh_filter_shift_output(filter);
        dh_filter_run_output_loop(filter);
        dh_filter_add_output(filter);
        // printf("//// outsetp %f current %f\n", input,filter->current_value);
    }

    if (output) {
        *output = filter->current_value;
    }

    return DH_FILTER_OK;
}


DH_FILTER_RETURN_VALUE dh_initialize_filter(dh_filter_data* filter, DH_FILTER_VALUE_TYPE value)
{
    assert(filter);
    if (!filter) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }
    for(size_t i=0; i< filter->number_coefficients_in; ++i) {
        filter->inputs[i] = value;
    }
    for(size_t i=0; i< filter->number_coefficients_out; ++i) {
        filter->outputs[i] = value;
    }
    filter->current_value = value;
    filter->initialized = true;
    return DH_FILTER_OK;
}

static void dh_filter_add_input(dh_filter_data* filter, DH_FILTER_VALUE_TYPE input)
{
    if (filter->current_input_index>0) {
        filter->current_input_index -= 1;
    }
    else {
        filter->current_input_index = filter->number_coefficients_in - 1U;
    }
    filter->inputs[filter->current_input_index] = input;
}

static double dh_filter_run_filter_loop(DH_FILTER_VALUE_TYPE* coefficients, size_t num_coeffs, DH_FILTER_VALUE_TYPE* data, size_t current_idx , size_t start)
{
    DH_FILTER_VALUE_TYPE out = 0.0;
    size_t split_loops = num_coeffs - current_idx;
    for(size_t i=start; i< split_loops; ++i) {
        size_t idx = current_idx + i;
        out += coefficients[i] * data[idx];
        // printf("l1 %f %f\n", coefficients[i],data[idx]);
    }
    for(size_t i=split_loops; i <  num_coeffs; ++i) {
        size_t idx = i-split_loops;
        out += coefficients[i] * data[idx];
        // printf("l2 %f %f\n", coefficients[i],data[idx]);
    }
    return out;
}

static void dh_filter_run_input_loop(dh_filter_data* filter)
{
    filter->current_value = dh_filter_run_filter_loop(filter->coefficients_in, filter->number_coefficients_in, filter->inputs, filter->current_input_index, 0);
}

static void dh_filter_run_output_loop(dh_filter_data* filter)
{
    filter->current_value -= dh_filter_run_filter_loop(filter->coefficients_out, filter->number_coefficients_out, filter->outputs, filter->current_output_index, 1);
}

static void dh_filter_shift_output(dh_filter_data* filter)
{
    if (filter->current_output_index>0) {
        filter->current_output_index -= 1;
    }
    else {
        filter->current_output_index = filter->number_coefficients_out - 1U;
    }
}

static void dh_filter_add_output(dh_filter_data* filter)
{
    filter->outputs[filter->current_output_index] = filter->current_value;
}
