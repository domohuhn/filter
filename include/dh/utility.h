#ifndef DH_UTILITY_H_INCLUDED
#define DH_UTILITY_H_INCLUDED

#include "dh/filter-types.h"
#include "dh/dh_complex.h"

/** @file
 * @brief Utilities for the filter library. 
 * 
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Computes positions of poles on the complex plane. All poles are equally spaced in quadrants 2 and 3.
 * The poles are computed on the unit circle and then scaled with the transformed frequency.
 * 
 * @param ptr pointer to output array
 * @param len number of poles to compute
 */
void dh_compute_poles_on_s_plane(COMPLEX* ptr, size_t len);

/**
 * @brief Computes polynomial coefficients from given array of roots.
 * 
 * @param roots array of roots
 * @param len number of roots +1/size of output array
 * @param outputs out struct - must have length len
 */
void dh_compute_polynomial_coefficients_from_roots(COMPLEX* roots, size_t len, COMPLEX* outputs);


/**
 * @brief Evaluates the polynomials at the given positions and normalizes the numerator for gain 1.0 at [x_evaluate].
 * 
 * @param numerator Array of polynomial coefficients. Will be modified in place.
 * @param len_numerator Number of entries in numerator array.
 * @param denominator Array of polynomial coefficients.
 * @param len_denominator Number of entries in denominator array.
 * @param x_evaluate Position where the polynominals are evaluated on the complex unit circle.
 */
void dh_normalize_gain_at(double* numerator, size_t len_numerator,double* denominator, size_t len_denominator, double x_evaluate);


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
 * @brief Convolves two sets of FIR filter parameters to combine them into one filter.
 * 
 * @param param1 Pointer to array with feedforward coefficients of the filter that is applied first.
 * @param param2 Pointer to array with feedforward coefficients of the filter that is applied second.
 * @param len Number of entries in array param1 and param2.
 * @param out The output is stored in that array. Must have 2*len-1 entries.
 * @return DH_FILTER_RETURN_VALUE 
 */
void dh_convolve_parameters(double * param1,double * param2, size_t len,double * out );

/** This struct holds the functions that are used to compute
 * the zeros and poles. */
typedef struct 
{
    /** Callback function to compute the zeros of a lowpass on the s-plane.
     * The resulting filter should have its cutoff at 1 rad/s.
     * 
     * Argument 1 : pointer to array
     * Argument 2 : Max size of the array
     * Argument 3 : Filter order
     * Argument 4 : user data
     * 
     * Returns the number initialized zeros.
     */
    size_t (*zeros)(COMPLEX*,size_t,size_t,void*);
    
    /** Callback function to compute the poles of a lowpass on the s-plane.
     * The resulting filter should have its cutoff at 1 rad/s.
     * 
     * Argument 1 : pointer to array
     * Argument 2 : Max size of the array
     * Argument 3 : Filter order
     * Argument 4 : user data
     * 
     * Returns the number initialized zeros.
     */
    size_t (*poles)(COMPLEX*,size_t,size_t,void*);
    
    /** User data for the functions */
    void* user_data;

    /** The filter characteristic */
    DH_FILTER_CHARACTERISTIC characteristic;
} dh_transfer_function_callbacks;

/**
 * @brief Computes the transfer function polynomial for a filter using the given callbacks.
 * 
 * An analog lowpass filter is created with the given callbacks in the [cbs] argument.
 * The roots are converted from an analog low pass to the desired characteristic and roots are appended
 * if needed. Then the roots are converted from the s-plane to the z-plane and the polynomial is expanded.
 * The real part of the polynomial coefficients is written to the coefficient arrays in [filter]. 
 * The order of the polynomial coefficients is inverted: index 0 is the coefficient for x**n.
 * 
 * This function will allocate temporary buffers.
 * 
 * @param filter The filter that will be initialized. Output values are written to the arrays.
 * @param options The options for the filter that are used to compute the values.
 * @param cbs Struct with the function pointers that are used to initialize the poles and zeros on the s-plane.
 * @return Returns DH_FILTER_OK on success, otherwise an error code is returned.
 */
DH_FILTER_RETURN_VALUE dh_compute_transfer_function_polynomials(dh_filter_data* filter, const dh_filter_parameters* options, const dh_transfer_function_callbacks cbs);

#ifdef __cplusplus
}
#endif

#endif /* DH_UTILITY_H_INCLUDED */
