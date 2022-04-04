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
 * @brief Computes all coefficients for a chebyshev low pass filter.
 * 
 * This function will allocate temporary buffers.
 * 
 * @param numerator Pointer to array with feedforward coefficients. Will be filled with output values. Must be filter_order+1 large.
 * @param denominator Pointer to array with feedback coefficients. Will be filled with output values. Must be filter_order+1 large.
 * @param filter_order Order of the filter.
 * @param cutoff_hz The desired 3db cutoff frequency in Hz. Must be larger than 0.
 * @param sampling_frequency_hz The sampling frequency in Hz. Must be larger than cutoff_hz.
 * @param ripple_db ripple of the filter. Must be a negative value in dB.
 * @return enum with status of operation 
 */
DH_FILTER_RETURN_VALUE compute_chebyshev_lowpass_coefficients(double* numerator, double* denominator, size_t filter_order,
        double cutoff_hz, double sampling_frequency_hz, double ripple_db);


/**
 * @brief Computes all coefficients for a butterworth high pass filter.
 * 
 * This function will allocate temporary buffers.
 * 
 * @param numerator Pointer to array with feedforward coefficients. Will be filled with output values. Must be filter_order+1 large.
 * @param denominator Pointer to array with feedback coefficients. Will be filled with output values. Must be filter_order+1 large.
 * @param filter_order Order of the filter.
 * @param cutoff_hz The desired 3db cutoff frequency in Hz. Must be larger than 0.
 * @param sampling_frequency_hz The sampling frequency in Hz. Must be larger than cutoff_hz.z.
 * @param ripple_db ripple of the filter. Must be a negative value in dB.
 * @return DH_FILTER_RETURN_VALUE 
 */
DH_FILTER_RETURN_VALUE compute_chebyshev_highpass_coefficients(double* numerator, double* denominator, size_t filter_order,
 double cutoff_hz, double sampling_frequency_hz, double ripple);


/**
 * @brief Computes the poles of the filter on the z-plane.
 * 
 * @param ptr pointer to array of complex numbers.
 * @param len length of the input array.
 * @param transformed_frequency the transfomed frequency
 * @param ripple_db the ripple value in db. Range [-inf, 0.0[
 */
void compute_chebyshev_poles_on_z_plane(COMPLEX* ptr, size_t len, double transformed_frequency, double ripple_db);

/**
 * @brief Computes the donimator polynomial for a chebyshev filter. These are the feedback coefficients.
 * 
 * @param ptr Pointer to array with coeffiecents. Must have filter_order+1 entries.
 * @param filter_order Order of the filter.
 * @param transformed_frequency Transformed frequency.
 * @param ripple_db the ripple value in db. Range [-inf, 0.0[
 * @return DH_FILTER_RETURN_VALUE 
 */
DH_FILTER_RETURN_VALUE compute_chebyshev_lowpass_denominator(double* ptr, size_t filter_order, double transformed_frequency, double ripple_db);

/**
 * @brief Transforms s-plane butterworth poles to chebyshev poles.
 * 
 * @param ptr Array with poles.
 * @param len Number of poles.
 * @param ripple_db desired ripple in db. Must be negative.
 */
void dh_transform_s_poles_to_chebyshev(COMPLEX* ptr, size_t len, double ripple_db);

#ifdef __cplusplus
}
#endif

#endif /* DH_CHEBYSHEV_H_INCLUDED */
