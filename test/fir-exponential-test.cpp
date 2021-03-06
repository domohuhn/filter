#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"
#include "dh/filter.h"
#include <cstring>

/**
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

SCENARIO( "An FIR exponential lowpass filter can be used", "[filter]" ) {

    GIVEN( "An options structure filled with values for FIR exponential lowpass filters" ) {
        dh_filter_data filter_data;
        dh_filter_parameters opts;
        opts.filter_type = DH_FIR_EXPONENTIAL_MOVING_AVERAGE_LOWPASS;
        opts.cutoff_frequency_low = 20;
        opts.sampling_frequency = 40;
        opts.filter_order = 15;

        WHEN( "the create function is called" ) {
            dh_create_filter(&filter_data, &opts);

            THEN( "the buffers are initialized" ) {
                REQUIRE(filter_data.buffer_needs_cleanup);
                REQUIRE(filter_data.number_coefficients_in == 16);
                REQUIRE(filter_data.number_coefficients_out == 1);
                REQUIRE((void*)filter_data.coefficients_in == (void*)filter_data.buffer);
                REQUIRE((void*)filter_data.inputs == (void*)(filter_data.buffer + 16 * sizeof(double)));
                REQUIRE((void*)filter_data.coefficients_out == (void*)(filter_data.buffer + 32 * sizeof(double)));
                REQUIRE((void*)filter_data.outputs == (void*)(filter_data.buffer + 33 * sizeof(double)));
                REQUIRE(filter_data.current_input_index == 0);
                REQUIRE(filter_data.current_output_index == 0);
                REQUIRE(filter_data.buffer_length == 34*sizeof(double));
                REQUIRE(filter_data.initialized == false);
            }
            double output=0.0;
            int status = dh_filter(&filter_data, 10.0, &output);

            THEN("The filter can be initialized") {
                REQUIRE(status == DH_FILTER_OK);
                REQUIRE(output == 10.0);
                REQUIRE(filter_data.current_value == output);
            }

            status = dh_filter(&filter_data, 20.0, &output);
            THEN("an impulse function can be filtered") {
                REQUIRE(status == DH_FILTER_OK);
                REQUIRE(output == Catch::Approx(15.0).margin(0.001));
                REQUIRE(filter_data.current_value == output);
            }

            status = dh_filter(&filter_data, 10.0, &output);
            THEN("an impulse function can be filtered") {
                REQUIRE(status == DH_FILTER_OK);
                REQUIRE(output == Catch::Approx(12.5).margin(0.001));
                REQUIRE(filter_data.current_value == output);
            }

            status = dh_filter(&filter_data, 10.0, &output);
            THEN("an impulse function can be filtered") {
                REQUIRE(status == DH_FILTER_OK);
                REQUIRE(output == Catch::Approx(11.25).margin(0.001));
                REQUIRE(filter_data.current_value == output);
            }

            status = dh_filter(&filter_data, 10.0, &output);
            THEN("an impulse function can be filtered") {
                REQUIRE(status == DH_FILTER_OK);
                REQUIRE(output == Catch::Approx(10.625).margin(0.001));
                REQUIRE(filter_data.current_value == output);
            }

            REQUIRE(dh_free_filter(&filter_data) == DH_FILTER_OK);
        }
    }
}

