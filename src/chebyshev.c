#include "dh/butterworth.h"
#include "dh/chebyshev.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "stdlib.h"
#include "complex.h"

void dh_transform_s_poles_to_chebyshev(COMPLEX* ptr, size_t len, double ripple) {
    double ripple_power = pow(10.0, -ripple / 10.0);
	double tmp = sqrt(ripple_power - 1.0);
	double arg = asinh(1.0 / tmp) / (double) len;
    for (int i = 0; i < len; i++)
	{ 
        double real = creal(ptr[i]);
        double imag = cimag(ptr[i]);
        ptr[i] = real*sinh(arg) + imag * cosh(arg) * I;
	}
}

void compute_chebyshev_poles_on_z_plane(COMPLEX* ptr, size_t len, double transformed_frequency, double ripple) 
{
    compute_poles_on_s_plane(ptr,len,transformed_frequency);
    dh_transform_s_poles_to_chebyshev(ptr,len,ripple);
    // invert poles for type 2
    for(size_t i=0; i<len; ++i) {
        ptr[i] = bilinear_z_transform(ptr[i]);
    }
}

DH_FILTER_RETURN_VALUE compute_chebyshev_lowpass_denominator(double* ptr, size_t filter_order, double transformed_frequency, double ripple)
{
    if(ptr == NULL || filter_order == 0) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }
    COMPLEX* poles = (COMPLEX*)malloc(sizeof(COMPLEX) * filter_order);
    if(poles == NULL) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    COMPLEX* polycoeff = (COMPLEX*)malloc(sizeof(COMPLEX) * (filter_order+1));
    if(polycoeff == NULL) {
        free(poles);
        return DH_FILTER_ALLOCATION_FAILED;
    }

    compute_chebyshev_poles_on_z_plane(poles, filter_order, transformed_frequency, ripple);

    size_t len = filter_order+1;
    compute_polynomial_coefficients_from_roots(poles, len, polycoeff);

    for(size_t i=0; i<len;++i){
        ptr[i] = creal(polycoeff[filter_order-i]);
    }

    free(polycoeff);
    free(poles);
    
    return DH_FILTER_OK;
}


DH_FILTER_RETURN_VALUE compute_chebyshev_lowpass_coefficients(double* numerator, double* denominator, size_t filter_order,
        double cutoff_hz, double sampling_frequency_hz, double ripple)
{
    if(numerator == NULL || denominator == NULL) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }
    double transformed = transform_frequency(cutoff_hz/sampling_frequency_hz);
    fill_array_with_binomial_coefficients(numerator,filter_order+1);
    DH_FILTER_RETURN_VALUE rv = compute_chebyshev_lowpass_denominator(denominator,filter_order,transformed,ripple);
    if(rv != DH_FILTER_OK) {
        return rv;
    }
    dh_normalize_gain_at(numerator,denominator,filter_order+1, 0.0);
    return DH_FILTER_OK;
}


DH_FILTER_RETURN_VALUE compute_chebyshev_highpass_coefficients(double* numerator, double* denominator, size_t filter_order, double cutoff_hz, double sampling_frequency_hz, double ripple)
{
    double mirrored_frequency = 0.5*sampling_frequency_hz - cutoff_hz;
    int rv = compute_chebyshev_lowpass_coefficients(numerator,denominator,filter_order,mirrored_frequency,sampling_frequency_hz, ripple);
    if(rv != DH_FILTER_OK) {
        return rv;
    }
    dh_alternate_signs(numerator,filter_order+1);
    dh_alternate_signs(denominator,filter_order+1);
    return rv;
}

DH_FILTER_RETURN_VALUE compute_chebyshev_bandfilter_coefficients(double* numerator, double* denominator, size_t filter_order, 
    double cutoff_low_hz, double cutoff_high_hz, double sampling_frequency_hz, bool bandpass, double ripple_db)
{
    return compute_butt_cheb_bandfilter_coefficients(numerator,denominator,filter_order,cutoff_low_hz,cutoff_high_hz,sampling_frequency_hz,bandpass,&ripple_db);
}

