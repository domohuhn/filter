#include "catch2/catch_test_macros.hpp"
#include "dh/filter.h"
#include <cstring>

/**
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

SCENARIO( "A moving average filter can be created", "[filter]" ) {

    GIVEN( "An options structure filled with values for moving average filters" ) {
        dh_filter_data moving_avg;
        dh_filter_parameters opts;
        opts.filter_type = DH_FIR_MOVING_AVERAGE_LOWPASS;
        opts.filter_order = 15;

        WHEN( "the create function is called" ) {
            dh_create_filter(&moving_avg, &opts);

            THEN( "the buffers are initialized" ) {
                REQUIRE(moving_avg.buffer_needs_cleanup);
                REQUIRE(moving_avg.number_coefficients_in == 16);
                REQUIRE(moving_avg.number_coefficients_out == 1);
                REQUIRE((void*)moving_avg.coefficients_in == (void*)moving_avg.buffer);
                REQUIRE((void*)moving_avg.inputs == (void*)(moving_avg.buffer + 16 * sizeof(double)));
                REQUIRE((void*)moving_avg.coefficients_out == (void*)(moving_avg.buffer + 32 * sizeof(double)));
                REQUIRE((void*)moving_avg.outputs == (void*)(moving_avg.buffer + 33 * sizeof(double)));
                REQUIRE(moving_avg.current_input_index == 0);
                REQUIRE(moving_avg.current_output_index == 0);
                REQUIRE(moving_avg.buffer_length == 34*sizeof(double));
                REQUIRE(moving_avg.initialized == false);
                for(size_t i=0;i<16;++i) {
                    REQUIRE(moving_avg.coefficients_in[i] == 1.0/16.0);
                }
            }
            double output=0.0;
            int status = dh_filter(&moving_avg, 10.0, &output);

            THEN("The filter can be initialized") {
                REQUIRE(status == DH_FILTER_OK);
                REQUIRE(output == 10.0);
                REQUIRE(moving_avg.current_value == output);
            }

            status = dh_filter(&moving_avg, 26.0, &output);
            THEN("a step function can be filtered") {
                REQUIRE(status == DH_FILTER_OK);
                REQUIRE(output == 11.0);
                REQUIRE(moving_avg.current_value == output);
            }

            REQUIRE(dh_free_filter(&moving_avg) == DH_FILTER_OK);
        }
    }
}


SCENARIO( "A moving average filter can be used", "[filter]" ) {

    GIVEN( "A moving average filter struct is manually created" ) {
        dh_filter_data moving_avg;
        moving_avg.initialized = true;
        moving_avg.current_input_index = 0;
        moving_avg.number_coefficients_in = 4;
        moving_avg.number_coefficients_out = 1;
        moving_avg.current_output_index = 0;
        double indata[4] = {0.0};
        double incoeffs[4] = {0.25, 0.25, 0.25, 0.25};
        double outcoeffs[1] = {1.0};
        moving_avg.inputs = indata;
        moving_avg.coefficients_in = incoeffs;
        moving_avg.coefficients_out = outcoeffs;

        WHEN( "a step function is filtered" ) {  
            double output=0.0;
            int status = dh_filter(&moving_avg, 0.0, &output);

            THEN( "the filter first returns zero" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 0.0);
            }

            output=0.0;
            status = dh_filter(&moving_avg, 20.0, &output);
            THEN( "the filter function is called with a step function the output has a linear response" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 5.0);
            }

            output=0.0;
            status = dh_filter(&moving_avg, 20.0, &output);
            THEN( "the filter function is called with a step function the output has a linear response" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 10.0);
            }

            output=0.0;
            status = dh_filter(&moving_avg, 20.0, &output);
            THEN( "the filter function is called with a step function the output has a linear response" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 15.0);
            }

            output=0.0;
            status = dh_filter(&moving_avg, 20.0, &output);
            THEN( "the filter function is called with a step function the output has a linear response" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 20.0);
            }

            output=0.0;
            status = dh_filter(&moving_avg, 20.0, &output);
            THEN( "the filter function reaches a constant state after 4 calls" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 20.0);
            }

            WHEN("The gain is set to 4.0") {
                status = dh_filter_set_gain(&moving_avg, 4.0);
                THEN( "the first value in the output coefficients is set to 4.0" ) {
                    REQUIRE( status == DH_FILTER_OK);
                    REQUIRE( outcoeffs[0] == 4.0);
                }
                
                WHEN("a filter cycle is run in the steady state") {
                    status = dh_filter(&moving_avg, 20.0, &output);
                    THEN( "the filter outputs 4 times the input" ) {
                        REQUIRE( status == DH_FILTER_OK);
                        REQUIRE( output == 80.0);
                    }
                }
            }
        }
    }
}



SCENARIO( "A moving average highpass filter can be used", "[filter]" ) {

    GIVEN( "An options structure filled with values for moving average filters" ) {
        dh_filter_data moving_avg;
        dh_filter_parameters opts;
        opts.filter_type = DH_FIR_MOVING_AVERAGE_HIGHPASS;
        opts.filter_order = 15;

        WHEN( "the create function is called" ) {
            dh_create_filter(&moving_avg, &opts);

            THEN( "the buffers are initialized" ) {
                REQUIRE(moving_avg.buffer_needs_cleanup);
                REQUIRE(moving_avg.number_coefficients_in == 16);
                REQUIRE(moving_avg.number_coefficients_out == 1);
                REQUIRE((void*)moving_avg.coefficients_in == (void*)moving_avg.buffer);
                REQUIRE((void*)moving_avg.inputs == (void*)(moving_avg.buffer + 16 * sizeof(double)));
                REQUIRE((void*)moving_avg.coefficients_out == (void*)(moving_avg.buffer + 32 * sizeof(double)));
                REQUIRE((void*)moving_avg.outputs == (void*)(moving_avg.buffer + 33 * sizeof(double)));
                REQUIRE(moving_avg.current_input_index == 0);
                REQUIRE(moving_avg.current_output_index == 0);
                REQUIRE(moving_avg.buffer_length == 34*sizeof(double));
                REQUIRE(moving_avg.initialized == true);
                REQUIRE(moving_avg.coefficients_in[0] == 1.0-1.0/16.0);
                for(size_t i=1;i<16;++i) {
                    REQUIRE(moving_avg.coefficients_in[i] == -1.0/16.0);
                }
            }
            double output=0.0;
            int status = dh_filter(&moving_avg, 10.0, &output);

            THEN("The filter can be initialized") {
                REQUIRE(status == DH_FILTER_OK);
                REQUIRE(output == 9.375);
                REQUIRE(moving_avg.current_value == output);
            }

            for(size_t i=0; i<30; ++i) {
                double last = output;
                status = dh_filter(&moving_avg, 10.0, &output);
                THEN("the output slowly drops to zero") {
                    REQUIRE(status == DH_FILTER_OK);
                    REQUIRE(output <= last);
                    REQUIRE(moving_avg.current_value == output);
                }
            }
            
            status = dh_filter(&moving_avg, 10.0, &output);
            THEN("the output reaches zero") {
                REQUIRE(status == DH_FILTER_OK);
                REQUIRE(output == 0.0);
                REQUIRE(moving_avg.current_value == output);
            }

            REQUIRE(dh_free_filter(&moving_avg) == DH_FILTER_OK);
        }
    }
}

