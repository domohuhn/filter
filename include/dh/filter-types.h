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

typedef struct {
    double* inputs;
    double* coefficients_in;
    double* outputs;
    double* coefficients_out;
    char* buffer;
    double current_value;
    size_t buffer_length;
    size_t number_coefficients_in;
    size_t number_coefficients_out;
    size_t current_input_index;
    size_t current_output_index;
    bool initialized;
    bool buffer_needs_cleanup;
} dh_filter_data;


typedef enum {
    DH_LOWPASS,
    DH_HIGHPASS,
    DH_BANDPASS,
    DH_BANDSTOP
} DH_FILTER_CHARACTERISTIC;

typedef struct {
    double cutoff_frequency_hz;
    double cutoff_frequency_2_hz;
    double sampling_frequency_hz;
    size_t filter_order;
} dh_butterworth_parameters;


typedef struct {
    double cutoff_frequency_hz;
    double cutoff_frequency_2_hz;
    double sampling_frequency_hz;
    size_t filter_order;
    double ripple;
} dh_chebyshev_parameters;


typedef enum {
    DH_NO_FILTER,
    DH_FIR_MOVING_AVERAGE,
    DH_FIR_MOVING_AVERAGE_HIGHPASS,
    DH_FIR_EXPONENTIAL_LOWPASS,
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

typedef struct {
    double alpha;
    size_t filter_order;
} dh_exponential_filter_parameters;


typedef struct {
    size_t filter_order;
} dh_moving_average_parameters;

typedef union {
    dh_exponential_filter_parameters exponential;
    dh_moving_average_parameters moving_average;
    dh_butterworth_parameters butterworth;
    dh_chebyshev_parameters chebyshev;
} dh_filter_parameters;

typedef struct {
    DH_FILTER_TYPE filter_type;
    dh_filter_parameters parameters;
} dh_filter_options;

typedef enum {
    DH_FILTER_OK,
    DH_FILTER_ERROR,
    DH_FILTER_NO_DATA_STRUCTURE,
    DH_FILTER_DATA_STRUCTURE_INPUTS_NOT_INITIALIZED,
    DH_FILTER_DATA_STRUCTURE_OUTPUTS_NOT_INITIALIZED,
    DH_FILTER_UNKNOWN_FILTER_TYPE,
    DH_FILTER_ALLOCATION_FAILED
} DH_FILTER_RETURN_VALUE;

#ifdef __cplusplus
}
#endif


#endif /* DH_FILTER_TYPES_H_INCLUDED */
