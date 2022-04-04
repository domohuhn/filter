
#include "dh/butterworth.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "stdlib.h"

void compute_butterworth_poles_on_z_plane(COMPLEX* ptr, size_t len, double transformed_frequency)
{
    compute_poles_on_s_plane(ptr,len,transformed_frequency);
    for(size_t i=0; i<len; ++i) {
        ptr[i] = bilinear_z_transform(ptr[i]);
    }
}

DH_FILTER_RETURN_VALUE compute_butterworth_lowpass_denominator(double* ptr, size_t filter_order, double transformed_frequency)
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

    compute_butterworth_poles_on_z_plane(poles, filter_order, transformed_frequency);

    size_t len = filter_order+1;
    compute_polynomial_coefficients_from_roots(poles, len, polycoeff);

    for(size_t i=0; i<len;++i){
        ptr[i] = creal(polycoeff[filter_order-i]);
    }

    free(polycoeff);
    free(poles);
    
    return DH_FILTER_OK;
}

DH_FILTER_RETURN_VALUE compute_butterworth_lowpass_coefficients(double* numerator, double* denominator, size_t filter_order, double cutoff_hz, double sampling_frequency_hz)
{
    if(numerator == NULL || denominator == NULL) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }
    double transformed = transform_frequency(cutoff_hz/sampling_frequency_hz);
    fill_array_with_binomial_coefficients(numerator,filter_order+1);
    DH_FILTER_RETURN_VALUE rv = compute_butterworth_lowpass_denominator(denominator,filter_order,transformed);
    if(rv != DH_FILTER_OK) {
        return rv;
    }
    dh_normalize_gain_at(numerator,denominator,filter_order+1, 0.0);
    return DH_FILTER_OK;
}

DH_FILTER_RETURN_VALUE compute_butterworth_highpass_coefficients(double* numerator, double* denominator, size_t filter_order, double cutoff_hz, double sampling_frequency_hz)
{
    if(numerator == NULL || denominator == NULL) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }
    double transformed = transform_frequency(cutoff_hz/sampling_frequency_hz);
    fill_array_with_binomial_coefficients(numerator,filter_order+1);
    DH_FILTER_RETURN_VALUE rv = compute_butterworth_lowpass_denominator(denominator,filter_order,transformed);
    if(rv != DH_FILTER_OK) {
        return rv;
    }
    dh_alternate_signs(numerator,filter_order+1);
    dh_normalize_gain_at(numerator,denominator,filter_order+1, 0.5);
    return rv;
}

DH_FILTER_RETURN_VALUE compute_butterworth_bandfilter_denominator(double* ptr, size_t filter_order, double transformed_frequency_low, double transformed_frequency_high, bool bandpass)
{    
    return compute_butt_cheb_bandfilter_denominator(ptr,filter_order,transformed_frequency_low,transformed_frequency_high,bandpass,NULL);
}

DH_FILTER_RETURN_VALUE compute_butterworth_bandfilter_coefficients(double* numerator, double* denominator, size_t filter_order, 
    double cutoff_low_hz, double cutoff_high_hz, double sampling_frequency_hz, bool bandpass)
{
    return compute_butt_cheb_bandfilter_coefficients(numerator,denominator,filter_order,cutoff_low_hz,cutoff_high_hz,sampling_frequency_hz,bandpass,NULL);
}
