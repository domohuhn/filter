#ifndef DH_FILTER_TYPES_H_INCLUDED
#define DH_FILTER_TYPES_H_INCLUDED

/** @file
 * @brief The data types for the filter library. 
 * 
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>
#include <stdbool.h>




typedef enum {
    DH_NO_FILTER,
    DH_FIR_MOVING_AVERAGE_LOWPASS,
    DH_FIR_MOVING_AVERAGE_HIGHPASS,
    DH_FIR_EXPONENTIAL_MOVING_AVERAGE_LOWPASS,
    DH_FIR_BRICKWALL_LOWPASS,
    DH_FIR_BRICKWALL_HIGHPASS,
    DH_FIR_BRICKWALL_BANDPASS,
    DH_FIR_BRICKWALL_BANDSTOP,
    DH_IIR_EXPONENTIAL_LOWPASS,
    DH_IIR_BUTTERWORTH_LOWPASS,
    DH_IIR_BUTTERWORTH_HIGHPASS,
    DH_IIR_BUTTERWORTH_BANDPASS,
    DH_IIR_BUTTERWORTH_BANDSTOP,
    DH_IIR_CHEBYSHEV_LOWPASS,
    DH_IIR_CHEBYSHEV_HIGHPASS,
    DH_IIR_CHEBYSHEV_BANDPASS,
    DH_IIR_CHEBYSHEV_BANDSTOP
} DH_FILTER_TYPE;




/**
 * The structure defining the parameters for a filter that will be created with dh_create_filter().
 */
typedef struct {
        /** The first cutoff frequency of the filter.
     * 
     * This value is used to configure the cutoff frequencies for
     * lowpass and highpass filters. For bandpass and bandstop filters, the value
     * represents the lower end (cutoff_frequency_low_hz<cutoff_frequency_high_hz must be true)
     * of the band.
     * 
     * This parameter is used for the following filters:
     * 
     * ```
     * DH_FIR_EXPONENTIAL_LOWPASS, DH_FIR_BRICKWALL_*, DH_IIR_EXPONENTIAL_*, DH_IIR_BUTTERWORTH_*, DH_IIR_CHEBYSHEV_*
     * ```
     *  
     * Valid Range : [0, sampling_frequency/2] or [0, cutoff_frequency_high/2]<br>
     * Unit: Any frequency - but cutoff_frequency_low, cutoff_frequency_high and sampling_frequency must have the same unit.
     **/
    double cutoff_frequency_low;

    /** The second cutoff frequency of the filter.
     * 
     * This value is used to configure the upper cutoff frequencies for bandpass and bandstop filters.
     * The value represents the upper end (cutoff_frequency_low_hz<cutoff_frequency_high_hz must be true)
     * of the band.
     * 
     * This parameter is used for the following filters:
     * 
     * ```
     * *_BANDPASS, *_BANDSTOP
     * ```
     *  
     * Valid Range : [cutoff_frequency_low, sampling_frequency/2]<br>
     * Unit: Any frequency - but cutoff_frequency_low, cutoff_frequency_high and sampling_frequency must have the same unit.
     **/
    double cutoff_frequency_high;

    /** The sampling rate for data supplied to the filter.
     * 
     * This value is used during configuration to scale the cutoff frequencies and then
     * compute the values. The filter itself does not depend on timings, and e.g. a lowpass
     * with 10Hz cutoff and 100Hz sampling rate will have the exact same parameters as one
     * with 20kHz cutoff and 200kHz sampling rate.
     * 
     * This parameter is used for the following filters:
     * 
     * ```
     * BRICKWALL, EXPONENTIAL, CHEBYSHEV, BUTTERWORTH
     * ```
     *  
     * Valid Range : [0, inf]<br>
     * Unit: Any frequency - but cutoff_frequency_low, cutoff_frequency_high and sampling_frequency must have the same unit.
     **/
    double sampling_frequency;

    /** The ripple of chebyshev filters.
     * 
     * This value is used during configuration to shift the poles of the chebyshev filters.
     * 
     * This parameter is used for the following filters:
     * 
     * ```
     * CHEBYSHEV
     * ```
     *  
     * Valid Range : [-inf, 0]
     * Unit: dB
     **/
    double ripple;

    /** The order of the filter.
     * 
     * This value determines the number of input and output coefficients in the recurrence relation
     * of the filter.
     * 
     * This parameter is used by every filter except:
     * 
     * ```
     * DH_NO_FILTER, DH_IIR_EXPONENTIAL_LOWPASS
     * ```
     *  
     * Valid Range : [0, inf]
     * Unit: none
     **/
    size_t filter_order;

    /** What filter to generate. Valid values are defined in the enum DH_FILTER_TYPE. */
    DH_FILTER_TYPE filter_type;
} dh_filter_options;

/** The return value for the public API of the library. */
typedef enum {
    /** Function was executed successfully. */
    DH_FILTER_OK,
    /** An undefined error. */
    DH_FILTER_ERROR,
    /** No input data given to the function. */
    DH_FILTER_NO_DATA_STRUCTURE,
    /** Given input data was not correctly initialized. */
    DH_FILTER_DATA_STRUCTURE_INPUTS_NOT_INITIALIZED,
    /** Given output data was not correctly initialized. */
    DH_FILTER_DATA_STRUCTURE_OUTPUTS_NOT_INITIALIZED,
    /** The requested filter type does not exist. */
    DH_FILTER_UNKNOWN_FILTER_TYPE,
    /** Allocation of the buffers failed or there was not enough space in the provided buffer. */
    DH_FILTER_ALLOCATION_FAILED
} DH_FILTER_RETURN_VALUE;

/** The interal data for a filter. */
typedef struct {
    /** Pointer to the array of the last inputs. Used as circular buffer. */
    double* inputs;
    /** Pointer to the array with the feedforward coefficients. */
    double* coefficients_in;
    /** Pointer to the array of the last outputs. Used as circular buffer. */
    double* outputs;
    /** Pointer to the array with the feedback coefficients. */
    double* coefficients_out;
    /** Pointer to the allocated buffer. */
    char* buffer;
    /** Current output value. */
    double current_value;
    /** Size of the buffer. */
    size_t buffer_length;
    /** Number of elements in inputs and coefficients_in. */
    size_t number_coefficients_in;
    /** Number of elements in outputs and coefficients_out. */
    size_t number_coefficients_out;
    /** Start index for the circular input buffer. */
    size_t current_input_index;
    /** Start index for the circular output buffer. */
    size_t current_output_index;
    /** If the filter was initialized. Relevant for low pass filters. */
    bool initialized;
    /** If the buffer needs to freed during free. */
    bool buffer_needs_cleanup;
} dh_filter_data;

#ifdef __cplusplus
}
#endif


#endif /* DH_FILTER_TYPES_H_INCLUDED */
