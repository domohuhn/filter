#ifndef DH_BUTTERWORTH_H_INCLUDED
#define DH_BUTTERWORTH_H_INCLUDED

/** @file
 * @brief The computation of butterworth coefficients for the filter library. 
 * 
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

#include "dh/filter-types.h"
#include "dh/utility.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Computes all coefficients for a butterworth low pass filter.
 * 
 * This function will allocate temporary buffers.
 * 
 * @param numerator Pointer to array with feedforward coefficients. Will be filled with output values. Must be filter_order+1 large.
 * @param denominator Pointer to array with feedback coefficients. Will be filled with output values. Must be filter_order+1 large.
 * @param filter_order Order of the filter.
 * @param cutoff_hz The desired 3db cutoff frequency in Hz. Must be larger than 0.
 * @param sampling_frequency_hz The sampling frequency in Hz. Must be larger than cutoff_hz.
 * @return DH_FILTER_RETURN_VALUE 
 */
DH_FILTER_RETURN_VALUE compute_butterworth_lowpass_coefficients(double* numerator, double* denominator, size_t filter_order, double cutoff_hz, double sampling_frequency_hz);


/**
 * @brief Computes all coefficients for a butterworth high pass filter.
 * 
 * This function will allocate temporary buffers.
 * 
 * @param numerator Pointer to array with feedforward coefficients. Will be filled with output values. Must be filter_order+1 large.
 * @param denominator Pointer to array with feedback coefficients. Will be filled with output values. Must be filter_order+1 large.
 * @param filter_order Order of the filter.
 * @param cutoff_hz The desired 3db cutoff frequency in Hz. Must be larger than 0.
 * @param sampling_frequency_hz The sampling frequency in Hz. Must be larger than cutoff_hz.
 * @return DH_FILTER_RETURN_VALUE 
 */
DH_FILTER_RETURN_VALUE compute_butterworth_highpass_coefficients(double* numerator, double* denominator, size_t filter_order, double cutoff_hz, double sampling_frequency_hz);

/**
 * @brief Computes all coefficients for a butterworth band pass or band stop filter.
 * 
 * This function will allocate temporary buffers.
 * 
 * @param numerator Pointer to array with feedforward coefficients. Will be filled with output values. Must be 2*filter_order+1 large.
 * @param denominator Pointer to array with feedback coefficients. Will be filled with output values. Must be 2*filter_order+1 large.
 * @param filter_order Order of the filter.
 * @param cutoff_low_hz The desired 3db cutoff frequency in Hz. Must be larger than 0.
 * @param cutoff_high_hz The desired 3db cutoff frequency in Hz. Must be larger than cutoff_low_hz.
 * @param sampling_frequency_hz The sampling frequency in Hz. Must be larger than 2*cutoff_high_hz.
 * @param bandpass if a bandpass or bandstop should be created.
 * @return DH_FILTER_RETURN_VALUE 
 */
DH_FILTER_RETURN_VALUE compute_butterworth_bandfilter_coefficients(double* numerator, double* denominator, size_t filter_order, 
    double cutoff_low_hz, double cutoff_high_hz, double sampling_frequency_hz, bool bandpass);


/**
 * @brief Computes the poles of the filter on the z-plane.
 * 
 * @param ptr pointer to array of complex numbers.
 * @param len length of the input array.
 * @param transformed_frequency the transfomed frequency
 */
void compute_butterworth_poles_on_z_plane(COMPLEX* ptr, size_t len, double transformed_frequency);

/**
 * @brief Computes the donimator polynomial for a butterworth filter. These are the feedback coefficients.
 * 
 * @param ptr Pointer to array with coefficients. Must have filter_order+1 entries.
 * @param filter_order Order of the filter.
 * @param transformed_frequency Transformed frequency.
 * @return DH_FILTER_RETURN_VALUE 
 */
DH_FILTER_RETURN_VALUE compute_butterworth_lowpass_denominator(double* ptr, size_t filter_order, double transformed_frequency);


/**
 * @brief Computes the donimator polynomial for a butterworth filter. These are the feedback coefficients.
 * 
 * @param ptr Pointer to array with coefficients. Must have 2*filter_order+1 entries.
 * @param filter_order Order of the filter.
 * @param transformed_frequency_low Transformed frequency.
 * @param transformed_frequency_high Transformed frequency.
 * @param bandpass if a bandpass is desired.
 * @return DH_FILTER_RETURN_VALUE 
 */
DH_FILTER_RETURN_VALUE compute_butterworth_bandfilter_denominator(double* ptr, size_t filter_order, double transformed_frequency_low, double transformed_frequency_high, bool bandpass);

#ifdef __cplusplus
}
#endif

#endif /* DH_BUTTERWORTH_H_INCLUDED */
