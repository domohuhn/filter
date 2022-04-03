#include "catch2/catch_test_macros.hpp"
#include "dh/filter.h"
#include <cstring>

/**
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

SCENARIO( "A moving average filter can be used", "[filter]" ) {

    GIVEN( "A moving average filter struct is manually created" ) {
        dh_filter_data moving_avg;
        moving_avg.initialized = true;
        moving_avg.current_input_index = 0;
        moving_avg.number_coefficients_in = 4;
        moving_avg.number_coefficients_out = 0;
        moving_avg.current_output_index = 0;
        DH_FILTER_VALUE_TYPE indata[4] = {0.0};
        DH_FILTER_VALUE_TYPE incoeffs[4] = {0.25, 0.25, 0.25, 0.25};
        moving_avg.inputs = indata;
        moving_avg.coefficients_in = incoeffs;

        WHEN( "a step function is filtered" ) {  
            DH_FILTER_VALUE_TYPE output=0.0;
            int status = dh_filter(&moving_avg, 0.0F, &output);

            THEN( "the filter first returns zero" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 0.0);
            }

            output=0.0;
            status = dh_filter(&moving_avg, 20.0F, &output);
            THEN( "the filter function is called with a step function the output has a linear response" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 5.0);
            }

            output=0.0;
            status = dh_filter(&moving_avg, 20.0F, &output);
            THEN( "the filter function is called with a step function the output has a linear response" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 10.0);
            }

            output=0.0;
            status = dh_filter(&moving_avg, 20.0F, &output);
            THEN( "the filter function is called with a step function the output has a linear response" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 15.0);
            }

            output=0.0;
            status = dh_filter(&moving_avg, 20.0F, &output);
            THEN( "the filter function is called with a step function the output has a linear response" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 20.0);
            }

            output=0.0;
            status = dh_filter(&moving_avg, 20.0F, &output);
            THEN( "the filter function reaches a constant state after 4 calls" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 20.0);
            }
        }

    }
}




