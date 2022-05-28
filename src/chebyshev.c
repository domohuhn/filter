#include "dh/butterworth.h"
#include "dh/chebyshev.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "stdlib.h"
#include "complex.h"
#include "stdio.h"

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
    // move zeros from infinity to stopband
    num_entries = append_to_array(lowpass,2*num_entries,center * I,append);
    return append_to_array(lowpass,num_entries,-center * I,append);
}

size_t shiftLowpassFrequency(COMPLEX* lowpass, size_t num_entries, double cutoff) {
    for(size_t i= 0; i<num_entries; ++i) {
        // shift from baseband to cutoff
        lowpass[i] *= cutoff;
    }
    return num_entries;
}

size_t lowpassToHighpass(COMPLEX* lowpass, size_t num_entries, double cutoff, size_t append) {
    for(size_t i= 0; i<num_entries; ++i) {
        // shift from baseband to cutoff
        lowpass[i] = cutoff/lowpass[i];
    }
    return append_to_array(lowpass,num_entries,0.0,append);
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


size_t compute_transferfunction_polynomial(DH_FILTER_CHARACTERISTIC type, COMPLEX* roots, size_t count,
                         double center, double width,
                         COMPLEX* polynomial, double* output,
                         size_t target_count){
    switch(type) {
    case DH_LOWPASS:
        count = shiftLowpassFrequency(roots,count,center);
        break;
    case DH_HIGHPASS:
        count = lowpassToHighpass(roots,count,center,target_count-count);
        break;
    case DH_BANDPASS:
        count = lowpassToBandpass(roots,count,center,width,target_count-count);
        target_count = 2*target_count;
        break;
    case DH_BANDSTOP:
        count = lowpassToBandstop(roots,count,center,width,target_count-count);
        target_count = 2*target_count;
        break;
    }

    count = bilinear_z_transform_and_append_ones(roots,count,2.0,target_count-count);
    size_t polylen = count+1;

    compute_polynomial_coefficients_from_roots(roots, polylen, polynomial);
    for(size_t i=0; i< polylen; ++i){
        output[polylen-1-i] = creal(polynomial[i]);
    }
    return polylen;
}


double position_for_gain(DH_FILTER_CHARACTERISTIC type, double cutoff_low_hz, double cutoff_high_hz, double sampling_frequency_hz) {
    switch(type) {
    case DH_LOWPASS: return 0.0;
    case DH_HIGHPASS: return 0.5;
    case DH_BANDPASS: return (0.5*cutoff_low_hz + 0.5*cutoff_high_hz)/sampling_frequency_hz;
    case DH_BANDSTOP: return 0.0;
    }
}

double compute_center(DH_FILTER_CHARACTERISTIC type, double warped_low, double warped_high){
    if(type == DH_BANDPASS || type == DH_BANDSTOP) {
        return sqrt(warped_low * warped_high);
    }
    return warped_low;
}

double compute_width(DH_FILTER_CHARACTERISTIC type, double warped_low, double warped_high){
    if(type == DH_BANDPASS || type == DH_BANDSTOP) {
        return warped_high - warped_low;
    }
    return 0.0;
}

DH_FILTER_RETURN_VALUE compute_chebyshev_filter_coefficients(DH_FILTER_CHARACTERISTIC type, double* numerator, double* denominator, size_t filter_order, 
    double cutoff_low_hz, double cutoff_high_hz, double sampling_frequency_hz, double ripple_db, bool isType2)
{
    if(numerator == NULL || denominator == NULL) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }

    double warped_low = 4 * transform_frequency(cutoff_low_hz/sampling_frequency_hz);
    double warped_high = 4 * transform_frequency(cutoff_high_hz/sampling_frequency_hz);
    
    double center = compute_center(type,warped_low,warped_high);
    double width = compute_width(type,warped_low,warped_high);

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
    if(isType2) {
        compute_zeros_on_s_plane_chebyshev2(splane, number_zeros, 1.0);
    } else {
        number_zeros = 0;
    }
    number_zeros = compute_transferfunction_polynomial(type, splane, number_zeros, center, width, polynomial, numerator, filter_order);
    
    // feedback coefficients
    compute_poles_on_s_plane(splane,number_poles,1.0);
    dh_transform_s_poles_to_chebyshev(splane,number_poles,ripple_db);
    // convert to chebyshev type 2
    if(isType2) {
        for(size_t i=0; i< number_poles; ++i){
            splane[i] = 1.0/splane[i];
        }
    }
    number_poles = compute_transferfunction_polynomial(type, splane, number_poles, center, width, polynomial, denominator, filter_order);

    free(buffer);

    // normalize gain to 1
    const double position = position_for_gain(type, cutoff_low_hz, cutoff_high_hz, sampling_frequency_hz);
    dh_normalize_gain_at(numerator,number_zeros,denominator,number_poles, position);
    return DH_FILTER_OK;
}

