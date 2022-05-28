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
 * @brief Computes all coefficients for a chebyshev high pass filter.
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
 * @brief Computes all coefficients for a chebyshev band pass or band stop filter.
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
 * @param ripple_db Ripple in db. Must be less than 0
 * @return DH_FILTER_RETURN_VALUE 
 */
DH_FILTER_RETURN_VALUE compute_chebyshev_bandfilter_coefficients(double* numerator, double* denominator, size_t filter_order, 
    double cutoff_low_hz, double cutoff_high_hz, double sampling_frequency_hz, bool bandpass, double ripple_db);

/**
 * @brief Computes the poles of the filter on the z-plane.
 * 
 * @param ptr pointer to array of complex numbers.
 * @param len length of the input array.
 * @param transformed_frequency the transfomed frequency
 * @param ripple_db the ripple value in db. Range [-inf, 0.0[
 * @param invert if the poles should be inverted to create a type 2 filter
 */
void compute_chebyshev_poles_on_z_plane(COMPLEX* ptr, size_t len, double transformed_frequency, double ripple_db, bool invert);

/**
 * @brief Computes the donimator polynomial for a chebyshev filter. These are the feedback coefficients.
 * 
 * @param ptr Pointer to array with coeffiecents. Must have filter_order+1 entries.
 * @param filter_order Order of the filter.
 * @param transformed_frequency Transformed frequency.
 * @param ripple_db the ripple value in db. Range [-inf, 0.0[
 * @param invert if the poles should be inverted to create a type 2 filter
 * @return DH_FILTER_RETURN_VALUE 
 */
DH_FILTER_RETURN_VALUE compute_chebyshev_lowpass_denominator(double* ptr, size_t filter_order, double transformed_frequency, double ripple_db, bool invert);

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
