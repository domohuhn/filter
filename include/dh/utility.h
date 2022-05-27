#ifndef DH_UTILITY_H_INCLUDED
#define DH_UTILITY_H_INCLUDED

#include "dh/filter-types.h"

/** @file
 * @brief Utilities for the filter library. 
 * 
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

#ifdef __cplusplus
extern "C" {
#endif

#define COMPLEX double _Complex

/**
 * @brief Computes teh bilinear z-transform of the input
 * 
 * @param p complex input number.
 * @return Transformed complex number.
 */
COMPLEX bilinear_z_transform(COMPLEX p);

/**
 * @brief Transforms the input value into the [0, Pi/2] range.
 * 
 * @param frequency The ratio of cutoff/sampling frequency in range [0,0.5].
 * @return transformed frequency
 */
double transform_frequency(double frequency);

/** Fills the given array [ptr] with the binomial coefficients of order [number-1].
 * E.g. for number 5 the array is filled with 1,4,6,4,1.
 */
void fill_array_with_binomial_coefficients(double* ptr, size_t number);

/**
 * @brief Computes positions of poles on the complex plane. All poles are equally spaced in quadrants 2 and 3.
 * The poles are computed on the unit circle and then scaled with the transformed frequency.
 * 
 * @param ptr pointer to output array
 * @param len number of poles to compute
 * @param transformed_frequency scaling factor applied to complex numbers
 */
void compute_poles_on_s_plane(COMPLEX* ptr, size_t len, double transformed_frequency);

/**
 * @brief Computes polynomial coefficients from given array of roots.
 * 
 * @param roots array of roots
 * @param len number of roots +1/size of output array
 * @param outputs out struct - must have length len
 */
void compute_polynomial_coefficients_from_roots(COMPLEX* roots, size_t len, COMPLEX* outputs);

/**
 * @brief Alternates the sign of the entries in the array.
 * 
 * The signs for every second element are inverted.
 * E.g. 1,2,3,4 becomes 1,-2,3,-4.
 * 
 * @param array pointer to array
 * @param len length of array
 */
void dh_alternate_signs(double* array, size_t len);

/**
 * @brief Generates the numerator polynomial for a bandpass.
 * 
 * All zeros are located on -1,+1 on the z-plane.
 * 
 * @param numerator Array with output values. Must be 2*order+1 in size.
 * @param order Order of the filter.
 * @return success of operation
 */
DH_FILTER_RETURN_VALUE dh_create_bandpass_numerator_polynomial(double* numerator, size_t order);

/**
 * @brief Generates the numerator polynomial for a bandstop.
 * 
 * All zeros are located on +- gm*i on the s-plane, where gm is the geomtetric mean of the two frequencies defining the band.
 * 
 * @param numerator Array with output values. Must be 2*order+1 in size.
 * @param order Order of the filter.
 * @param gm Geotmetric mean of band frequencies divided by sampling frequency.
 * @return success of operation
 */
DH_FILTER_RETURN_VALUE dh_create_bandstop_numerator_polynomial(double* numerator, size_t order, double gm);

/**
 * @brief Transforms poles to a bandpass or bandstop filter.
 * The number of poles doubles, so the the poles array must have extra space!
 * 
 * @param poles Array of s-plane poles. Must have length 2*len.
 * @param len Length of existing poles.
 * @param transformed_frequency_low Lower frequency of band (transformed).
 * @param transformed_frequency_high Upper frequency of band (transformed).
 * @param bandpass If a bandpass is desired. Otherwise bandstop is assumed.
 */
void dh_transform_poles_bandfilter(COMPLEX* poles, size_t len, double transformed_frequency_low,  double transformed_frequency_high, bool bandpass);


/**
 * @brief Computes the donimator polynomial for a butterworth filter. These are the feedback coefficients.
 * 
 * @param ptr Pointer to array with coefficients. Must have 2*filter_order+1 entries.
 * @param filter_order Order of the filter.
 * @param transformed_frequency_low Transformed frequency.
 * @param transformed_frequency_high Transformed frequency.
 * @param bandpass if a bandpass is desired.
 * @param ripple_db Ripple in db. If NULL, butterworth filter values are returned. If this value is set, poles are tranformed to chebyshev poles.
 * @return DH_FILTER_RETURN_VALUE 
 */
DH_FILTER_RETURN_VALUE compute_butt_cheb_bandfilter_denominator(double* ptr, size_t filter_order, double transformed_frequency_low,
    double transformed_frequency_high, bool bandpass, double* ripple_db);

/**
 * @brief Evaluates the polynomials at the given positions and normalizes the numerator for gain 1.0 at [x_evaluate].
 * 
 * @param numerator Array of polynomial coefficients. Will be modified in place.
 * @param denominator Array of polynomial coefficients.
 * @param len Number of entries.
 * @param x_evaluate Position where the polynominals are evaluated on the complex unit circle.
 */
void dh_normalize_gain_at(double* numerator,double* denominator,size_t len, double x_evaluate);


/**
 * @brief Evaluates the polynomials at the given position and returns the gain.
 * 
 * @param numerator Array of polynomial coefficients.
 * @param len_numerator Number of entries in numerator array.
 * @param denominator Array of polynomial coefficients.
 * @param len_denominator Number of entries in denominator array.
 * @param x_evaluate Position where the polynominals are evaluated on the complex unit circle.
 */
COMPLEX dh_gain_at(double* numerator, size_t len_numerator,double* denominator, size_t len_denominator, double x_evaluate);


/**
 * @brief Computes all coefficients for a butterworth or chebyshev band pass or band stop filter.
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
 * @param ripple_db Ripple in db. If NULL, butterworth filter values are returned. If this value is set, poles are tranformed to chebyshev poles.
 * @return DH_FILTER_RETURN_VALUE 
 */
DH_FILTER_RETURN_VALUE compute_butt_cheb_bandfilter_coefficients(double* numerator, double* denominator, size_t filter_order, 
    double cutoff_low_hz, double cutoff_high_hz, double sampling_frequency_hz, bool bandpass, double* ripple_db);

/**
 * @brief Convolves two sets of FIR filter parameters to combine them into one filter.
 * 
 * @param param1 Pointer to array with feedforward coefficients of the filter that is applied first.
 * @param param2 Pointer to array with feedforward coefficients of the filter that is applied second.
 * @param len Number of entries in array param1 and param2.
 * @param out The output is stored in that array. Must have 2*len-1 entries.
 * @return DH_FILTER_RETURN_VALUE 
 */
void convolve_parameters(double * param1,double * param2, size_t len,double * out );

/**
 * @brief Computes the zeros on the s-plane for a chebyshev type 2 filter.
 * 
 * @param ptr Array to store the zeros in
 * @param len number of zeros
 * @param transformed_frequency prewarped frequency
 */
void compute_zeros_on_s_plane_chebyshev2(COMPLEX* ptr, size_t len, double transformed_frequency);

#ifdef __cplusplus
}
#endif

#endif /* DH_UTILITY_H_INCLUDED */
