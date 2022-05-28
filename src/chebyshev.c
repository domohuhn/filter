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

void compute_chebyshev_poles_on_z_plane(COMPLEX* ptr, size_t len, double transformed_frequency, double ripple, bool invert) 
{
    compute_poles_on_s_plane(ptr,len,1.0);
    dh_transform_s_poles_to_chebyshev(ptr,len,ripple);
    for(size_t i=0; i<len; ++i) {
        ptr[i] = bilinear_z_transform(invert ? transformed_frequency/ptr[i] : transformed_frequency*ptr[i]);
    }
}

DH_FILTER_RETURN_VALUE compute_chebyshev_lowpass_denominator(double* ptr, size_t filter_order, double transformed_frequency, double ripple, bool invert)
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

    compute_chebyshev_poles_on_z_plane(poles, filter_order, transformed_frequency, ripple, invert);

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
    DH_FILTER_RETURN_VALUE rv = compute_chebyshev_lowpass_denominator(denominator,filter_order,transformed,ripple,false);
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


DH_FILTER_RETURN_VALUE compute_chebyshev2_lowpass_coefficients(double* numerator, double* denominator, size_t filter_order,
        double cutoff_hz, double sampling_frequency_hz, double ripple)
{
    if(numerator == NULL || denominator == NULL) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }
    double transformed = transform_frequency(cutoff_hz/sampling_frequency_hz);
    DH_FILTER_RETURN_VALUE rv = compute_chebyshev_lowpass_denominator(denominator,filter_order,transformed,ripple,true);

    COMPLEX* zeros = (COMPLEX*)malloc(sizeof(COMPLEX) * filter_order);
    if(zeros == NULL) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    size_t polylen = filter_order+1;
    COMPLEX* polynomial = (COMPLEX*)malloc(sizeof(COMPLEX) * polylen);
    if(polynomial == NULL) {
        free(zeros);
        return DH_FILTER_ALLOCATION_FAILED;
    }
    compute_zeros_on_s_plane_chebyshev2(zeros , filter_order ,transformed);
    for(size_t i=0; i<filter_order; ++i) {
        zeros[i] = bilinear_z_transform(zeros[i]);
    }

    compute_polynomial_coefficients_from_roots(zeros, polylen, polynomial);

    for(size_t i=0; i<polylen;++i){
        numerator[i] = creal(polynomial[filter_order-i]);
    }

    free(polynomial);
    free(zeros);

    if(rv != DH_FILTER_OK) {
        return rv;
    }
    dh_normalize_gain_at(numerator,denominator,filter_order+1, 0.0);
    return DH_FILTER_OK;
}

DH_FILTER_RETURN_VALUE compute_chebyshev2_highpass_coefficients(double* numerator, double* denominator, size_t filter_order, double cutoff_hz, double sampling_frequency_hz, double ripple)
{
    double mirrored_frequency = 0.5*sampling_frequency_hz - cutoff_hz;
    int rv = compute_chebyshev2_lowpass_coefficients(numerator,denominator,filter_order,mirrored_frequency,sampling_frequency_hz, ripple);
    if(rv != DH_FILTER_OK) {
        return rv;
    }
    dh_alternate_signs(numerator,filter_order+1);
    dh_alternate_signs(denominator,filter_order+1);
    return rv;
}


size_t append_to_array(COMPLEX* lowpass, size_t num_entries, COMPLEX value, size_t append_number){
    for(size_t i= 0; i<append_number; ++i){
        lowpass[num_entries +i] = value;
    }
    return num_entries+append_number;
}

// zeros needs twice the size of the number of input zeros
// append = number of roots at zero to add. (or the number of poles - number of zeros)
// returns the new number in the array
size_t lowpassToBandpass(COMPLEX* lowpass, size_t num_entries, double center, double width, size_t append) {
    for(size_t i= 0; i<num_entries; ++i) {
        // scale to bandwidth
        COMPLEX current =  width/2.0 * lowpass[i];
        // duplicate and shift from baseband to +center and -center
        lowpass[i] = current + csqrt( current*current - center*center);
        lowpass[i + num_entries] = current - csqrt( current*current - center*center);
    }
    return append_to_array(lowpass,2*num_entries,0.0,append);
}

size_t lowpassToBandstop(COMPLEX* lowpass, size_t num_entries, double center, double width, size_t append) {
    for(size_t i= 0; i<num_entries; ++i) {
        // invert to highpass
        COMPLEX current =  (width*0.5) / lowpass[i];
        // duplicate and shift from baseband to +center and -center
        lowpass[i] = current + csqrt( current*current - center*center);
        lowpass[i + num_entries] = current - csqrt( current*current - center*center);
    }
    return append_to_array(lowpass,2*num_entries,0.0,append);
}


// append = number of roots at -1.0 to add (Nyquist frequency). (or the number of poles - number of zeros)
// returns the new number in the array
size_t bilinear_z_transform_and_append_ones(COMPLEX* splane, size_t num_entries, double fsample, size_t append) {
    const double fsample2 = 2 * fsample;
    for(size_t i= 0; i<num_entries; ++i) {
        splane[i] = (fsample2 + splane[i]) / (fsample2 - splane[i]);
    }
    return append_to_array(splane,num_entries,-1.0,append);
}

DH_FILTER_RETURN_VALUE compute_chebyshev2_bandfilter_coefficients(double* numerator, double* denominator, size_t filter_order, 
    double cutoff_low_hz, double cutoff_high_hz, double sampling_frequency_hz, bool bandpass, double ripple_db)
{
    if(numerator == NULL || denominator == NULL) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }

    double warped_low = 4 * transform_frequency(cutoff_low_hz/sampling_frequency_hz);
    double warped_high = 4 * transform_frequency(cutoff_high_hz/sampling_frequency_hz);
    
    double center = sqrt(warped_low * warped_high);
    double width = warped_high - warped_low;

    // allocate temporary buffers
    size_t buffer_length = 4*filter_order+1;
    COMPLEX* buffer = (COMPLEX*)malloc(sizeof(COMPLEX) * buffer_length);
    if(buffer == NULL) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    COMPLEX* splane = buffer;
    size_t number_zeros = filter_order;
    size_t number_poles = filter_order;
    size_t polylen = 2*filter_order+1;
    COMPLEX* polynomial = buffer + 2*filter_order;

    // feedforward coefficients
    compute_zeros_on_s_plane_chebyshev2(splane, number_zeros, 1.0);
/*     for(size_t i=0; i< number_zeros; ++i){
        printf("zeros s-plane %zu : %f+%f*i\n",i,creal(splane[i]),cimag(splane[i]));
    } */
    if(bandpass) {
        number_zeros = lowpassToBandpass(splane,number_zeros,center,width,0);
    } else {
        number_zeros = lowpassToBandstop(splane,number_zeros,center,width,0);
    }

    number_zeros = bilinear_z_transform_and_append_ones(splane,number_zeros,2.0,0);
/*     for(size_t i=0; i< number_zeros; ++i){
        printf("zeros z-plane %zu : %f+%f*i\n",i,creal(splane[i]),cimag(splane[i]));
    } */

    compute_polynomial_coefficients_from_roots(splane, polylen, polynomial);
    for(size_t i=0; i< polylen; ++i){
//        printf("copy num %zu\n",i);
        numerator[i] = creal(polynomial[i]);
    }
    
    // feedback coefficients
    
    compute_poles_on_s_plane(splane,number_poles,1.0);
    dh_transform_s_poles_to_chebyshev(splane,number_poles,ripple_db);
    // convert to chebyshev type 2
    for(size_t i=0; i< number_poles; ++i){
        splane[i] = 1.0/splane[i];
    }
/*     for(size_t i=0; i< number_poles; ++i){
        printf("poles s-plane %zu : %f+%f*i\n",i,creal(splane[i]),cimag(splane[i]));
    } */

    if(bandpass) {
        number_poles = lowpassToBandpass(splane,number_poles,center,width,0);
    } else {
        number_poles = lowpassToBandstop(splane,number_poles,center,width,0);
    }
    number_poles = bilinear_z_transform_and_append_ones(splane,number_poles,2.0,0);
/*     for(size_t i=0; i< number_zeros; ++i){
        printf("poles z-plane %zu : %f+%f*i\n",i,creal(splane[i]),cimag(splane[i]));
    } */
    
    compute_polynomial_coefficients_from_roots(splane, polylen, polynomial);
    for(size_t i=0; i< polylen; ++i){
        //printf("copy deno %zu\n",i);
        denominator[polylen-1-i] = creal(polynomial[i]);
    }

    free(buffer);
    // normalize gain to 1 at 0 for bandstop, center of passband for bandpass
    const double normalize_gain_at = bandpass ? (0.5*cutoff_low_hz + 0.5*cutoff_high_hz)/sampling_frequency_hz : 0.0;
    dh_normalize_gain_at(numerator,denominator,polylen, normalize_gain_at);
    return DH_FILTER_OK;
}


