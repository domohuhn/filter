#include "dh/filter.h"
#include <stdlib.h>

/**
 * @file 
 * @brief This file contains the code to clean up a filter.
 * 
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

DH_FILTER_RETURN_VALUE dh_free_filter(dh_filter_data* filter)
{
    if(filter != NULL) {
        if(filter->buffer_needs_cleanup) {
            free(filter->buffer);
        }
        filter->inputs = NULL;
        filter->coefficients_in = NULL;
        filter->outputs = NULL;
        filter->coefficients_out = NULL;
        filter->buffer = NULL;
        filter->current_value = 0.0;
        filter->buffer_length = 0;
        filter->number_coefficients_in = 0;
        filter->number_coefficients_out = 0;
        filter->current_input_index = 0;
        filter->current_output_index = 0;
        filter->initialized = false;
        filter->buffer_needs_cleanup = false;
    }
    return DH_FILTER_OK;
}

