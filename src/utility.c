#include "dh/utility.h"
#include "dh/chebyshev.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "stdlib.h"
#include "complex.h"

COMPLEX bilinear_z_transform(COMPLEX p) {
    return (1.0 + p)/(1.0 -p);
}

static COMPLEX complex_unit_circle(double phi) {
    return cos(phi) + sin(phi) *I;
}

void fill_array_with_binomial_coefficients(double* ptr, size_t number)
{
    if ( ptr==NULL || number==0 ) {
        return;
    }
    size_t binom_base = number - 1;
    ptr[0] = 1.0;
    if(number > 1) {
        ptr[1] = binom_base;
        size_t mid = binom_base/2;
        for (size_t i=2; i<=mid; ++i) {
            ptr[i] = ptr[i-1] * (double)(number-i)/(double)i;
        }
        for (size_t i = mid+1; i<number; ++i) {
            ptr[i] = ptr[binom_base-i];
        }
    }
}

void dh_alternate_signs(double* array, size_t len){
    if (array == NULL) {
        return;
    }
    for(size_t i=0; i<len; ++i) {
        array[i] = i%2==0 ? array[i] : -array[i];
    }
}

double transform_frequency(double frequency)
{
    return tan(frequency * M_PI);
}

void compute_poles_on_s_plane(COMPLEX* ptr, size_t len, double transformed_frequency) {
    for(size_t i=0; i<len; ++i) {
        double phi = (2*i+1)*M_PI/(2*len) + M_PI/2.0;
        ptr[i] = transformed_frequency * complex_unit_circle(phi);
    }
}

void compute_polynomial_coefficients_from_roots(COMPLEX* roots, size_t len, COMPLEX* outputs)
{
    if ( roots==(void*)NULL|| len==0 || outputs==(void*)NULL ) {
        return;
    }
    outputs[0] = -roots[0];
    outputs[1] = 1.0;
    for(size_t i=2; i<len; ++i) {
        outputs[i] = outputs[i-1];
        for(size_t k=i-1; k>=1; --k) {
            outputs[k] = outputs[k-1] - outputs[k]*roots[i-1];
        }
        outputs[0] = -outputs[0]*roots[i-1];
    }
}

static double accumulate_array(double* ptr, size_t len)
{
    double sum = 0.0;
    for(size_t i=0; i<len; ++i){
        sum += ptr[i];
    }
    return sum;
}

DH_FILTER_RETURN_VALUE dh_create_bandpass_numerator_polynomial(double* numerator, size_t order)
{
    COMPLEX* roots = (COMPLEX*)malloc(sizeof(COMPLEX) * 2*order);
    if(roots == NULL) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    COMPLEX* polynomial = (COMPLEX*)malloc(sizeof(COMPLEX) * (2*order+1));
    if(polynomial == NULL) {
        free(roots);
        return DH_FILTER_ALLOCATION_FAILED;
    }
    for (size_t i=0;i<order;++i) {
        roots[i] = 1.0;
        roots[order+i] = -1.0;
    }
    compute_polynomial_coefficients_from_roots(roots , 2*order+1 ,polynomial);
    for (size_t i=0; i<(2*order+1);++i) {
        numerator[i] = creal(polynomial[2*order-i]);
    }
    free(polynomial);
    free(roots);
    return DH_FILTER_OK;
}

DH_FILTER_RETURN_VALUE dh_create_bandstop_numerator_polynomial(double* numerator, size_t order, double gm)
{
    COMPLEX* roots = (COMPLEX*)malloc(sizeof(COMPLEX) * 2*order);
    if(roots == NULL) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    COMPLEX* polynomial = (COMPLEX*)malloc(sizeof(COMPLEX) * (2*order+1));
    if(polynomial == NULL) {
        free(roots);
        return DH_FILTER_ALLOCATION_FAILED;
    }
    COMPLEX positive = bilinear_z_transform(gm * I);
    COMPLEX negative = bilinear_z_transform(-gm * I);
    for (size_t i=0;i<order;++i) {
        roots[i] = positive;
        roots[order+i] = negative;
    }
    compute_polynomial_coefficients_from_roots(roots , 2*order+1 ,polynomial);
    for (size_t i=0; i<(2*order+1);++i) {
        numerator[i] = creal(polynomial[i]);
    }
    free(polynomial);
    free(roots);
    return DH_FILTER_OK;
}

void dh_transform_poles_bandfilter(COMPLEX* poles, size_t len, double transformed_frequency_low,  double transformed_frequency_high, bool bandpass)
{
    if (poles == (void*)NULL) {
        return;
    }
    double geo_mean = sqrt(transformed_frequency_low * transformed_frequency_high);
    double geo_mean_sqr = geo_mean*geo_mean;
    double half_difference = 0.5*(transformed_frequency_high - transformed_frequency_low);

    for(size_t i =0; i<len; ++i) {
        COMPLEX v1;
        if (bandpass) {
            v1 = half_difference * poles[i];
        } else {
            v1 = half_difference / poles[i];
        }
        COMPLEX v2 = csqrt( 1.0 - geo_mean_sqr/(v1*v1));
        poles[i] = v1 * ( 1.0 + v2 );
        poles[i+len] = v1 * ( 1.0 - v2 );
    }
}

DH_FILTER_RETURN_VALUE compute_butt_cheb_bandfilter_denominator(double* ptr, size_t filter_order, double transformed_frequency_low, double transformed_frequency_high, bool bandpass, double* ripple_db)
{
    if(ptr == NULL || filter_order == 0) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }
    COMPLEX* poles = (COMPLEX*)malloc(sizeof(COMPLEX) * (2*filter_order));
    if(poles == NULL) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    COMPLEX* polycoeff = (COMPLEX*)malloc(sizeof(COMPLEX) * (2*filter_order+1));
    if(polycoeff == NULL) {
        free(poles);
        return DH_FILTER_ALLOCATION_FAILED;
    }
    compute_poles_on_s_plane(poles,filter_order,1.0);
    if(ripple_db != (void*)NULL) {
        dh_transform_s_poles_to_chebyshev(poles,filter_order,*ripple_db);
    }
    dh_transform_poles_bandfilter(poles,filter_order,transformed_frequency_low,transformed_frequency_high,bandpass);
    for(size_t i=0; i<2*filter_order; ++i) {
        poles[i] = bilinear_z_transform(poles[i]);
    }

    size_t len = 2*filter_order+1;
    compute_polynomial_coefficients_from_roots(poles, len, polycoeff);

    for(size_t i=0; i<len;++i){
        ptr[i] = creal(polycoeff[len-1-i]);
    }
    
    free(polycoeff);
    free(poles);
    
    return DH_FILTER_OK;
}

static COMPLEX evaluate_polynomial(double* coeffs, size_t len, COMPLEX x) 
{
    COMPLEX rv = 0.0;
    for(size_t i=0; i<len; ++i) {
        rv = rv*x + coeffs[i];
    }
    return rv;
}


COMPLEX dh_gain_at(double* numerator, size_t len_numerator,double* denominator, size_t len_denominator, double x_evaluate)
{
    if(numerator == NULL || denominator == NULL) {
        return 1.0;
    }
    COMPLEX arg = complex_unit_circle(2*M_PI*x_evaluate);
    COMPLEX num = evaluate_polynomial(numerator,len_numerator,arg);
    COMPLEX denom = evaluate_polynomial(denominator,len_denominator,arg);
    return num/denom;
}

void dh_normalize_gain_at(double* numerator,double* denominator,size_t len, double x_evaluate)
{
    if(numerator == NULL || denominator == NULL) {
        return;
    }
    double scale = cabs(1.0/dh_gain_at(numerator,len,denominator,len,x_evaluate));
    for(size_t i=0; i<len; ++i) {
        numerator[i] *= scale;
    }
}

void convolve_parameters(double * param1,double * param2, size_t len,double * out ) {
    if(param1 == NULL || param2 == NULL || out==NULL) {
        return;
    }
    for(size_t i=0;i<2*len-1; ++i) {
        out[i] = 0.0;
    }
    for(size_t i=0;i<len; ++i) {
        for(size_t k=0;k<len; ++k) {
            out[i+k] += param1[i] * param2[k];
        }
    }
}

DH_FILTER_RETURN_VALUE compute_butt_cheb_bandfilter_coefficients(double* numerator, double* denominator, size_t filter_order, 
    double cutoff_low_hz, double cutoff_high_hz, double sampling_frequency_hz, bool bandpass, double* ripple_db)
{
    if(numerator == NULL || denominator == NULL) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }
    double transformed_low = transform_frequency(cutoff_low_hz/sampling_frequency_hz);
    double transformed_high = transform_frequency(cutoff_high_hz/sampling_frequency_hz);

    DH_FILTER_RETURN_VALUE rv;
    if(bandpass) {
        rv = dh_create_bandpass_numerator_polynomial(numerator,filter_order);
    } else {
        rv = dh_create_bandstop_numerator_polynomial(numerator,filter_order, sqrt(transformed_high*transformed_low));
    }
    if (rv != DH_FILTER_OK) {
        return rv;
    }

    rv = compute_butt_cheb_bandfilter_denominator(denominator,filter_order,transformed_low, transformed_high, bandpass, ripple_db);
    if(rv != DH_FILTER_OK) {
        return rv;
    }
    double center = bandpass ? (0.5*cutoff_low_hz + 0.5*cutoff_high_hz)/sampling_frequency_hz : 0.0;
    dh_normalize_gain_at(numerator,denominator,2*filter_order+1, center);
    return DH_FILTER_OK;
}
