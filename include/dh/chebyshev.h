#ifndef DH_CHEBYSHEV_H_INCLUDED
#define DH_CHEBYSHEV_H_INCLUDED

/** @file
 * @brief The computation of chebyshev coefficients for the filter library. 
 * 
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

#include "dh/filter-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Computes all coefficients for a chebyshev filter.
 * 
 * This function will allocate temporary buffers.
 * 
 * @param filter The data structure with the filter parameters. It will be modified in place (the coefficient arrays are initialized).
 * @param options The selected options for the filter.
 * @param characteristic The type of chebyshev filter that will be created.
 * @param isType2 if the filter should be a type 2 filter.
 * @return DH_FILTER_RETURN_VALUE 
 */
DH_FILTER_RETURN_VALUE dh_compute_chebyshev_filter_coefficients(dh_filter_data* filter, dh_filter_parameters* options, DH_FILTER_CHARACTERISTIC characteristic, bool isType2);
    

#ifdef __cplusplus
}
#endif

#endif /* DH_CHEBYSHEV_H_INCLUDED */
