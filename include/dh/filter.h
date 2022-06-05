#ifndef DH_FILTER_H_INCLUDED
#define DH_FILTER_H_INCLUDED

/** @file
 * @brief The primary include file for the filter library. 
 * 
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "dh/filter-types.h"

/**
 * @mainpage
 * 
 * This is the mainpage of the API documentation for the FilterGen library. 
 * The library provides the functionality to design and use finite and infinite
 * impulse response filters in C and C++. The source code can be found on <a href="https://github.com/domohuhn/filter">github</a>
 * and is governed by the MIT license.
 * 
 * @section com Compilation
 * 
 * In order to compile the library you need cmake and a working C-Compiler:
 * ```bash
 * git clone https://github.com/domohuhn/filter.git
 * cd filter && mkdir build && cd build
 * cmake -DCMAKE_BUILD_TYPE='Release' -DCMAKE_INSTALL_PREFIX='<somewhere>' ..
 * make install
 * ```
 * 
 * The library can be configured with the following options during the CMake call:
 * 
 * CMake option  | Default value | Description
 * ------------- | ------------- | ------------
 * DH_CFILTER_BUILD_CPP_BINDINGS  | ON  |  If the bindings for c++ should be built.
 * DH_CFILTER_BUILD_JS_BINDINGS   | OFF  |  If the bindings for javascript should be built. This includes the C++ Bindings. Requires emscripten.
 * DH_CFILTER_BUILD_TESTS  | ON  |  If the tests should be built. Will fetch Catch2.
 * DH_CFILTER_BUILD_EXAMPLES  | ON  | If the examples should be built. Will fetch CXXopts.
 * 
 * @section pak CMake package
 * 
 * After the library has bee installed, you can use the installed package in CMake:
 * 
 * ```bash
 * find_package(FilterGen)
 * # using the package:
 * target_link_libraries(some_target_c PRIVATE dh::filter)
 * target_link_libraries(some_target_cpp PRIVATE dh::filter_cpp)
 * ```
 * 
 * @section use API Descriptions
 * 
 * For a usage example see either the examples directory in the repository, or check the modules for each supported programming language listed below:
 * 
 *  - @ref C-API
 *  - @ref cpp-API
 * 
 * 
 * @section javascript Javascript bindings
 * 
 * The library can be compiled to Webassembly with emscripten and it provides bindings to javascript via the C++-API.
 * In order to create the webassembly target, set the following options during the cmake call:
 * ```bash
 * cmake -DDH_CFILTER_BUILD_TESTS='OFF' \
 *       -DDH_CFILTER_BUILD_EXAMPLES='OFF' \
 *       -DDH_CFILTER_BUILD_JS_BINDINGS='ON' \
 *       <path to source>
 * ```
 * 
 * You can see the javascript bindings in action on <a href="https://domohuhn.github.io/filter/">github pages.</a>
 * 
 * @note This documentation was generated with doxygen and the stylesheets from <a href="https://github.com/jothepro/doxygen-awesome-css">doxygen awesome.</a>
 */

/**
 * @defgroup C-API API for C
 * 
 * @brief The API for the C Programming language.
 * 
 * The API of the filter library is kept as simple as possible. When you create a filter,
 * you first create a dh_filter_parameters structure and initialize its data members.
 * The filter must be initialized via dh_create_filter().
 * 
 * Here is an example for the basic usage of the API:
 * ```C
 * dh_filter_parameters parameters;
 * // set values in parameters to create your desired filter
 * dh_filter_data filter;
 * if (dh_create_filter(&filter, &parameters) != DH_FILTER_OK) {
 *     // handle error
 * }
 * // the filter is ready to be used
 * while (do_stuff) {
 *     double output = 0.0;
 *     double input = get_input();
 *     if (dh_filter(&filter, input, &output) == DH_FILTER_OK) {
 *         // use output
 *     } else { 
 *         // handle error
 *     }
 * }
 * // cleanup after you are done
 * dh_free_filter(&filter);
 * ```
 */


/**
 * @brief Runs an iteration of the filter.
 * 
 * The function runs a loop for the feedforward and feedback coefficients. The ring buffer holding the past inputs and outputs
 * will be updated.
 * @note If the initialized property of the filter structure is set to false, then all past input and output values are set to
 * the given input and the property is set to true. This only makes sense if the gain at 0 Hz is supposed to be 1 (e.g. lowpass or bandstop filters).
 * It prevents the "initialization time" in case you filter objects that may appear and disappear.
 * 
 * @param[in] filter The data structure of the filter. Must be initialized (the buffers/coefficients must be set).
 * @param[in] input The next input value to the filter.
 * @param[out] output The current output value. Parameter is optional. If it is not NULL, then the output value is written to the given address.
 * @return An enum with the result of the operation.
 * @retval DH_FILTER_OK Operation was successfull
 * @retval DH_FILTER_NO_DATA_STRUCTURE You gave NULL as first argument.
 * @retval DH_FILTER_DATA_STRUCTURE_NOT_INITIALIZED The filter data structure was not correctly initialized.
 * @ingroup C-API
 */
DH_FILTER_RETURN_VALUE dh_filter(dh_filter_data* filter, double input, double* output);

/**
 * @brief Allocates the buffers and initializes the filter.
 * 
 * A filter generated by this function will always have gain 1. 
 * If you need a different gain, call dh_filter_set_gain().
 * 
 * 
 * @param[out] filter pointer to the filter structure that will be initialized.
 * @param[in] options the desired filter type.
 * @return DH_FILTER_RETURN_VALUE 
 * @retval DH_FILTER_OK Operation was successfull
 * @retval DH_FILTER_NO_DATA_STRUCTURE You gave NULL as first argument.
 * @retval DH_FILTER_UNKNOWN_FILTER_TYPE An unknown filter was requested in the options.
 * @retval DH_FILTER_ALLOCATION_FAILED Not enough memory for the filter could be allocated.
 * @see dh_filter_parameters for the possible options.
 * @ingroup C-API
 */
DH_FILTER_RETURN_VALUE dh_create_filter(dh_filter_data* filter, dh_filter_parameters* options);

/**
 * @brief Forces the filter to the steady state with output value by setting all pasts inputs and outputs to the given [value].
 * 
 * @note Call this function only with low-pass or band-stop filters! Usually, you do no not have to call this yourself if you use dh_create_filter().
 * 
 * @param[in] filter the filter structure
 * @param[in] value the desired steady state
 * @return An enum with the result of the operation.
 * @retval DH_FILTER_OK Operation was successfull
 * @retval DH_FILTER_NO_DATA_STRUCTURE You gave NULL as first argument.
 * @ingroup C-API
 */
DH_FILTER_RETURN_VALUE dh_initialize_filter(dh_filter_data* filter, double value);

/** Frees the filter created with dh_create_filter().
 * 
 * If the filter is marked as owner of the buffer, free() will be called to clean up the allocated memory.
 * @param[in] filter the filter structure that will be freed.
 * @return An enum with the result of the operation.
 * @retval DH_FILTER_OK Operation was successfull
 * @ingroup C-API
 */
DH_FILTER_RETURN_VALUE dh_free_filter(dh_filter_data* filter);

/** Sets the gain of the filter to the given value.
 * 
 * @param[in] filter the filter structure
 * @param[in] gain the desired gain
 * @return An enum with the result of the operation.
 * @retval DH_FILTER_OK Operation was successfull
 * @retval DH_FILTER_NO_DATA_STRUCTURE You gave NULL as first argument.
 * @retval DH_FILTER_DATA_STRUCTURE_NOT_INITIALIZED The filter data structure was not correctly initialized.
 * @ingroup C-API
 */
DH_FILTER_RETURN_VALUE dh_filter_set_gain(dh_filter_data* filter, double gain);

/** Gets the gain of the filter.
 * 
 * @param[in] filter the filter structure
 * @param[out] gain pointer to output
 * @return An enum with the result of the operation.
 * @retval DH_FILTER_OK Operation was successfull
 * @retval DH_FILTER_NO_DATA_STRUCTURE You gave NULL as first argument.
 * @retval DH_FILTER_DATA_STRUCTURE_NOT_INITIALIZED The filter data structure was not correctly initialized.
 * @ingroup C-API
 */
DH_FILTER_RETURN_VALUE dh_filter_get_gain(const dh_filter_data* filter, double* gain);

/** Gets the gain of the filter.
 * 
 * @param[in] filter the filter structure
 * @param[in] frequency frequency/sampling_frequency where the gain is computed. Range: [0,0.5]
 * @param[out] gain pointer to output
 * @return An enum with the result of the operation.
 * @retval DH_FILTER_OK Operation was successfull
 * @retval DH_FILTER_NO_DATA_STRUCTURE You gave NULL as first argument.
 * @retval DH_FILTER_DATA_STRUCTURE_NOT_INITIALIZED The filter data structure was not correctly initialized.
 * @ingroup C-API
 */
DH_FILTER_RETURN_VALUE dh_filter_get_gain_at(const dh_filter_data* filter, double frequency, dh_frequency_response_t* gain);


#ifdef __cplusplus
}
#endif

#endif /* DH_FILTER_H_INCLUDED */
