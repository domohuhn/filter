#include "dh/utility.h"
#include "dh/chebyshev.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "stdlib.h"
#include "complex.h"

/** Return a complex number on the complex unit circle located at the angle [phi]. */
static COMPLEX complex_unit_circle(double phi) {
    return cos(phi) + sin(phi) *I;
}

/**
 * @brief Transforms the input value into the [0, Pi/2] range.
 * 
 * @param frequency The ratio of cutoff/sampling frequency in range [0,0.5].
 * @return transformed frequency
 */
static double transform_frequency(double frequency)
{
    return tan(frequency * M_PI);
}

void dh_compute_poles_on_s_plane(COMPLEX* ptr, size_t len) {
    for(size_t i=0; i<len; ++i) {
        double phi = (2*i+1)*M_PI/(2*len) + M_PI/2.0;
        ptr[i] = complex_unit_circle(phi);
    }
}

void dh_compute_polynomial_coefficients_from_roots(COMPLEX* roots, size_t len, COMPLEX* outputs)
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

/**
 * @brief Evaluates a polynomial.
 * 
 * @param coeffs Pointer to array with the given coefficients.
 *               The highest index in the array is the coefficient for x**0.
 * @param len Number of coefficients in the array.
 * @param x Position where the polynomial is evaluated.
 * @return computed value 
 */
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

void dh_normalize_gain_at(double* numerator, size_t len_numerator,double* denominator, size_t len_denominator, double x_evaluate)
{
    if(numerator == NULL || denominator == NULL) {
        return;
    }
    double scale = cabs(1.0/dh_gain_at(numerator,len_numerator,denominator,len_denominator,x_evaluate));
    for(size_t i=0; i<len_numerator; ++i) {
        numerator[i] *= scale;
    }
}

void dh_convolve_parameters(double * param1,double * param2, size_t len,double * out ) {
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

/**
 * @brief Appends the [value] multiple times to the given array.
 * 
 * @param array Array of complex numbers. The array must be at least skip_entries+append_number large.
 * @param skip_entries The number of entries to skip in front.
 * @param value Value to write.
 * @param append_number Number of times the value is written.
 * @return Index of the element one past the last written value. 
 */
static size_t append_to_array(COMPLEX* array, size_t skip_entries, COMPLEX value, size_t append_number){
    for(size_t i= 0; i<append_number; ++i){
        array[skip_entries +i] = value;
    }
    return skip_entries+append_number;
}

/**
 * @brief Converts an analog lowpass to a bandpass.
 * 
 * @param lowpass Pointer to array with the lowpass coefficients. The entries will be modified in place.
 *        The array must have 2*num_entries+append elements.
 * @param num_entries Number of initialized lowpass coefficients in the array.
 * @param center Center of the band.
 * @param width Width of the band.
 * @param append The number of roots to append. Set this to the number_of_poles - number_of_zeros for zeros, 0 for poles.
 * @return Index of the element one past the last written value.  
 */
static size_t lowpassToBandpass(COMPLEX* lowpass, size_t num_entries, double center, double width, size_t append) {
    for(size_t i= 0; i<num_entries; ++i) {
        // scale to bandwidth
        COMPLEX current =  width/2.0 * lowpass[i];
        // duplicate and shift from baseband to +center and -center
        lowpass[i] = current + csqrt( current*current - center*center);
        lowpass[i + num_entries] = current - csqrt( current*current - center*center);
    }
    return append_to_array(lowpass,2*num_entries,0.0,append);
}

/**
 * @brief Converts an analog lowpass to a bandstop.
 * 
 * @param lowpass Pointer to array with the lowpass coefficients. The entries will be modified in place.
 *        The array must have 2*num_entries+append elements.
 * @param num_entries Number of initialized lowpass coefficients in the array.
 * @param center Center of the band.
 * @param width Width of the band.
 * @param append The number of roots to append. Set this to the number_of_poles - number_of_zeros for zeros, 0 for poles.
 * @return Index of the element one past the last written value.  
 */
static size_t lowpassToBandstop(COMPLEX* lowpass, size_t num_entries, double center, double width, size_t append) {
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

/**
 * @brief Shifts the cutoff frequency of a lowpass filter from 1.0 to cutoff.
 * 
 * @param lowpass Pointer to array with the lowpass coefficients. The entries will be modified in place.
 * @param num_entries Number of initialized lowpass coefficients in the array.
 * @param cutoff The cutoff frequency of the high pass.
 * @return Index of the element one past the last written value.  
 */
static size_t shiftLowpassFrequency(COMPLEX* lowpass, size_t num_entries, double cutoff) {
    for(size_t i= 0; i<num_entries; ++i) {
        // shift from baseband to cutoff
        lowpass[i] *= cutoff;
    }
    return num_entries;
}

/**
 * @brief Converts an analog lowpass to a bandstop.
 * 
 * @param lowpass Pointer to array with the lowpass coefficients. The entries will be modified in place.
 *        The array must have num_entries+append elements.
 * @param num_entries Number of initialized lowpass coefficients in the array.
 * @param cutoff The cutoff frequency of the high pass.
 * @param append The number of roots to append. Set this to the number_of_poles - number_of_zeros for zeros, 0 for poles.
 * @return Index of the element one past the last written value.  
 */
static size_t lowpassToHighpass(COMPLEX* lowpass, size_t num_entries, double cutoff, size_t append) {
    for(size_t i= 0; i<num_entries; ++i) {
        // shift from baseband to cutoff
        lowpass[i] = cutoff/lowpass[i];
    }
    return append_to_array(lowpass,num_entries,0.0,append);
}


/**
 * @brief Computes the bilinear z-transform of the inputs in [splane].
 * 
 * Also appends [append_entries] roots at -1.0 (at the Nyquist frequency) to the array.
 * @param splane array of complex numbers.
 * @param num_entries number of array elements.
 * @param fsample sampling frequency.
 * @param append_entries The number of roots to append. Set this to the number_of_poles - number_of_zeros.
 * @return The new number of entries in the array.
 */
static size_t bilinear_z_transform_and_append_ones(COMPLEX* splane, size_t num_entries, double fsample, size_t append_entries) {
    const double fsample2 = 2 * fsample;
    for(size_t i= 0; i<num_entries; ++i) {
        splane[i] = (fsample2 + splane[i]) / (fsample2 - splane[i]);
    }
    return append_to_array(splane,num_entries,-1.0,append_entries);
}

/**
 * @brief Computes the transfer function polynomial for a filter with the given roots.
 * 
 * The roots are converted from an analog low pass to the desired characteristic and roots are appended
 * if needed. Then the roots are converted from the s-plane to the z-plane and the polynomial is expanded.
 * The real part of the polynomial coefficients is written to [output]. The order of the polynomial coefficients
 * is inverted: index 0 is the coefficient for x**n.
 * 
 * @param type The type of the filter.
 * @param roots Array with complex roots of the polynomial on the s-plane for an analog lowpass.
 * @param count Number of roots.
 * @param center Center frequency of the filter.
 * @param width Width of the band of the filter. Ignored for lowpass and highpass.
 * @param polynomial Pointer to the buffer where the polynomial is expanded. Needs at least count+1 entries. 
 * @param output Pointer to the array where the outputs are written to. Needs at least count+1 entries.
 * @param target_count The target number of roots. Additional roots will be appended if there are not enough.
 * @return size_t Number of elements written to output.
 */
static size_t compute_transferfunction_polynomial(DH_FILTER_CHARACTERISTIC type, COMPLEX* roots, size_t count,
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

    dh_compute_polynomial_coefficients_from_roots(roots, polylen, polynomial);
    for(size_t i=0; i< polylen; ++i){
        output[polylen-1-i] = creal(polynomial[i]);
    }
    return polylen;
}

/**
 * @brief Selects the method to compute the frequency where the gain will be set to 1.0.
 * @return Computed position 
 */
static double normalize_at_frequency(DH_FILTER_CHARACTERISTIC type, double cutoff_low_hz, double cutoff_high_hz, double sampling_frequency_hz) {
    switch(type) {
    case DH_HIGHPASS: return 0.5;
    case DH_BANDPASS: return (0.5*cutoff_low_hz + 0.5*cutoff_high_hz)/sampling_frequency_hz;
    default: return 0.0;
    }
}

/**
 * @brief Computes the center of the interval [warped_low,warped_high].
 * @return Computed value 
 */
static double compute_center(DH_FILTER_CHARACTERISTIC type, double warped_low, double warped_high){
    if(type == DH_BANDPASS || type == DH_BANDSTOP) {
        return sqrt(warped_low * warped_high);
    }
    return warped_low;
}

/**
 * @brief Computes the width of the interval [warped_low,warped_high].
 * @return Computed value 
 */
static double compute_width(DH_FILTER_CHARACTERISTIC type, double warped_low, double warped_high){
    if(type == DH_BANDPASS || type == DH_BANDSTOP) {
        return warped_high - warped_low;
    }
    return 0.0;
}

DH_FILTER_RETURN_VALUE dh_compute_transfer_function_polynomials(dh_filter_data* filter, const dh_filter_parameters* options, const dh_transfer_function_callbacks cbs)
{
    if(filter == NULL || options == NULL || filter->coefficients_in == NULL || filter->coefficients_out == NULL || cbs.zeros == NULL || cbs.poles == NULL) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }
    const DH_FILTER_CHARACTERISTIC type = cbs.characteristic;
    const size_t filter_order = options->filter_order;
    double* numerator = filter->coefficients_in;
    double* denominator = filter->coefficients_out;
    const double cutoff_frequency_low =  options->cutoff_frequency_low;
    const double cutoff_frequency_high =  options->cutoff_frequency_high;
    const double sampling_frequency =  options->sampling_frequency;
    
    double warped_low = 4 * transform_frequency(cutoff_frequency_low/sampling_frequency);
    double warped_high = 4 * transform_frequency(cutoff_frequency_high/sampling_frequency);
    
    double center = compute_center(type,warped_low,warped_high);
    double width = compute_width(type,warped_low,warped_high);

    // allocate temporary buffers
    size_t buffer_length = 4*filter_order+1;
    COMPLEX* buffer = (COMPLEX*)malloc(sizeof(COMPLEX) * buffer_length);
    if(buffer == NULL) {
        return DH_FILTER_ALLOCATION_FAILED;
    }
    COMPLEX* splane = buffer;
    COMPLEX* polynomial = buffer + 2*filter_order;
    size_t polylen = 2*(filter_order)+1;

    // feedforward coefficients
    size_t number_zeros = cbs.zeros(splane,polylen,filter_order,cbs.user_data);
    number_zeros = compute_transferfunction_polynomial(type, splane, number_zeros, center, width, polynomial, numerator, filter_order);
    
    // feedback coefficients
    size_t number_poles = cbs.poles(splane,polylen,filter_order,cbs.user_data);
    number_poles = compute_transferfunction_polynomial(type, splane, number_poles, center, width, polynomial, denominator, filter_order);

    free(buffer);

    // normalize gain to 1
    const double frequency = normalize_at_frequency(type, cutoff_frequency_low, cutoff_frequency_high, sampling_frequency);
    dh_normalize_gain_at(numerator,number_zeros,denominator,number_poles, frequency);
    return DH_FILTER_OK;
}
