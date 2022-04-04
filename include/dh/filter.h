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

#include "dh/filter-types.h"


/**
 * @brief Runs an iteration of the filter.
 * 
 * 
 * @param[in] filter The data structure of the filter. Must be initialized (the buffers/coefficients must be set).
 * @param[in] input The next input value to the filter.
 * @param[out] output The current output value. Parameter is optional. If set, the output value is written to the given address.
 * @return DH_FILTER_RETURN_VALUE with the result of the filter operation. 
 */
DH_FILTER_RETURN_VALUE dh_filter(dh_filter_data* filter, double input, double* output);

/**
 * @brief Allocates the buffers and initializes the filter.
 * 
 * @param filter the filter structure
 * @param options the desired filter type
 * @return DH_FILTER_RETURN_VALUE 
 */
DH_FILTER_RETURN_VALUE dh_create_filter(dh_filter_data* filter, dh_filter_options* options);

/**
 * @brief Forces the filter to the steady state with output value.
 * 
 * @param filter the filter structure
 * @param value the desired steady state
 * @return DH_FILTER_RETURN_VALUE 
 */
DH_FILTER_RETURN_VALUE dh_initialize_filter(dh_filter_data* filter, double value);


DH_FILTER_RETURN_VALUE dh_free_filter(dh_filter_data* filter);

//DH_FILTER_RETURN_VALUE dh_create_filter_with_external_buffer(dh_filter_data* filter, dh_filter_options* options, char* buffer, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* DH_FILTER_H_INCLUDED */
