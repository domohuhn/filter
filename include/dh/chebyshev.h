#ifndef DH_CHEBYSHEV_H_INCLUDED
#define DH_CHEBYSHEV_H_INCLUDED

/** @file
 * @brief The computation of chebyshev coefficients for the filter library. 
 * 
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

#include "dh/filter-types.h"
#include "dh/utility.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Transforms s-plane butterworth poles to chebyshev poles.
 * 
 * @param ptr Array with poles.
 * @param len Number of poles.
 * @param ripple_db desired ripple in db. Must be negative.
 */
void dh_transform_s_poles_to_chebyshev(COMPLEX* ptr, size_t len, double ripple_db);

/**
 * @brief Computes all coefficients for a chebyshev type 2 band pass or band stop filter.
 * 
 * This function will allocate temporary buffers.
 * 
 * @param type what kind of filter should be created.
 * @param numerator Pointer to array with feedforward coefficients. Will be filled with output values. 
 *                  Must have filter_order+1 entries for lowpass/highpass, or 2*filter_order+1 for band filters.
 * @param denominator Pointer to array with feedback coefficients. Will be filled with output values.
 *                  Must have filter_order+1 entries for lowpass/highpass, or 2*filter_order+1 for band filters.
 * @param filter_order Order of the filter.
 * @param cutoff_low_hz The desired 3db cutoff frequency in Hz. Must be larger than 0.
 * @param cutoff_high_hz The desired 3db cutoff frequency in Hz. Must be larger than cutoff_low_hz. Ignored for lowpass/highpass.
 * @param sampling_frequency_hz The sampling frequency in Hz. Must be larger than 2*cutoff_high_hz.
 * @param ripple_db Ripple in db.
 * @param isType2 if the filter should be a type 2 filter.
 * @return DH_FILTER_RETURN_VALUE 
 */
DH_FILTER_RETURN_VALUE compute_chebyshev_filter_coefficients(DH_FILTER_CHARACTERISTIC type, double* numerator, double* denominator, size_t filter_order, 
    double cutoff_low_hz, double cutoff_high_hz, double sampling_frequency_hz, double ripple_db, bool isType2);

#ifdef __cplusplus
}
#endif

#endif /* DH_CHEBYSHEV_H_INCLUDED */
