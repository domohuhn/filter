#ifndef DH_FILTER_H_INCLUDED
#define DH_FILTER_H_INCLUDED

/** @file
 * @brief The primary include file for the filter library. 
 * 
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>

#ifndef DH_FILTER_VALUE_TYPE
#define DH_FILTER_VALUE_TYPE double
#endif /* DH_FILTER_VALUE_TYPE */

typedef struct {
    DH_FILTER_VALUE_TYPE* inputs;
    DH_FILTER_VALUE_TYPE* coefficients_in;
    DH_FILTER_VALUE_TYPE* outputs;
    DH_FILTER_VALUE_TYPE* coefficients_out;
    DH_FILTER_VALUE_TYPE current_value;
    char* buffer;
    size_t buffer_length;
    size_t number_coefficients_in;
    size_t number_coefficients_out;
    size_t current_input_index;
    size_t current_output_index;
    bool initialized;

} dh_filter_data;


typedef enum {
    DH_FILTER_OK,
    DH_FILTER_ERROR,
    DH_FILTER_NO_DATA_STRUCTURE,
    DH_FILTER_DATA_STRUCTURE_INPUTS_NOT_INITIALIZED,
    DH_FILTER_DATA_STRUCTURE_OUTPUTS_NOT_INITIALIZED
} DH_FILTER_RETURN_VALUE;

/**
 * @brief Runs an iteration of the filter.
 * 
 * 
 * @param[in] filter The data structure of the filter. Must be initialized (the buffers/coefficients must be set).
 * @param[in] input The next input value to the filter.
 * @param[out] output The current output value. Parameter is optional. If set, the output value is written to the given address.
 * @return DH_FILTER_RETURN_VALUE with the result of the filter operation. 
 */
DH_FILTER_RETURN_VALUE dh_filter(dh_filter_data* filter, DH_FILTER_VALUE_TYPE input, DH_FILTER_VALUE_TYPE* output);

#ifdef __cplusplus
}
#endif

#endif /* DH_FILTER_H_INCLUDED */
