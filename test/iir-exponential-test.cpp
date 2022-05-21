#include "catch2/catch_test_macros.hpp"
#include "dh/filter.h"
#include <cstring>

/**
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

SCENARIO( "An IIR exponential lowpass filter can be used", "[filter]" ) {

    GIVEN( "An options structure filled with values for IIR exponential lowpass filters" ) {
        dh_filter_data iir_exp;
        dh_filter_parameters opts;
        opts.filter_type = DH_IIR_EXPONENTIAL_LOWPASS;
        opts.cutoff_frequency_low = 10;
        opts.sampling_frequency = 40;

        WHEN( "the create function is called" ) {
            dh_create_filter(&iir_exp, &opts);

            THEN( "the buffers are initialized" ) {
                REQUIRE(iir_exp.buffer_needs_cleanup);
                REQUIRE(iir_exp.number_coefficients_in == 1);
                REQUIRE(iir_exp.number_coefficients_out == 2);
                REQUIRE((void*)iir_exp.coefficients_in == (void*)iir_exp.buffer);
                REQUIRE((void*)iir_exp.inputs == (iir_exp.buffer + 1 * sizeof(double)));
                REQUIRE((void*)iir_exp.coefficients_out == (iir_exp.buffer + 2 * sizeof(double)));
                REQUIRE((void*)iir_exp.outputs == (iir_exp.buffer + 4 * sizeof(double)));
                REQUIRE(iir_exp.current_input_index == 0);
                REQUIRE(iir_exp.current_output_index == 0);
                REQUIRE(iir_exp.buffer_length == 6*sizeof(double));
                REQUIRE(iir_exp.initialized == false);
                REQUIRE(iir_exp.coefficients_in[0] == 0.25);
                REQUIRE(iir_exp.coefficients_out[0] == 1.0);
                REQUIRE(iir_exp.coefficients_out[1] == -0.75);
            }
            double output=0.0;
            int status = dh_filter(&iir_exp, 10.0, &output);

            THEN("The filter can be initialized") {
                REQUIRE(status == DH_FILTER_OK);
                REQUIRE(output == 10.0);
                REQUIRE(iir_exp.current_value == output);
            }

            status = dh_filter(&iir_exp, 20.0, &output);
            THEN("a step function can be filtered") {
                REQUIRE(status == DH_FILTER_OK);
                REQUIRE(output == 12.5);
                REQUIRE(iir_exp.current_value == output);
            }

            status = dh_filter(&iir_exp, 22.5, &output);
            THEN("a step function can be filtered") {
                REQUIRE(status == DH_FILTER_OK);
                REQUIRE(output == 15.0);
                REQUIRE(iir_exp.current_value == output);
            }

            REQUIRE(dh_free_filter(&iir_exp) == DH_FILTER_OK);
        }
    }
}

