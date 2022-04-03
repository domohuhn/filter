#include "catch2/catch_test_macros.hpp"
#include "dh/filter.h"
#include <cstring>

/**
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

SCENARIO( "The coefficients are used in correct order", "[filter]" ) {
    GIVEN( "A filter struct is manually created with different input parameters" ) {
        dh_filter_data filter_data;
        filter_data.initialized = true;
        filter_data.current_input_index = 0;
        filter_data.number_coefficients_in = 4;
        filter_data.number_coefficients_out = 0;
        filter_data.current_output_index = 0;
        DH_FILTER_VALUE_TYPE indata[4] = {0.0};
        DH_FILTER_VALUE_TYPE incoeffs[4] = {-0.5, 0.25, -0.75, -1.5};
        filter_data.inputs = indata;
        filter_data.coefficients_in = incoeffs;

        WHEN( "an impulse input is filtered" ) {  
            DH_FILTER_VALUE_TYPE output=0.0;
            int status = dh_filter(&filter_data, 20.0F, &output);

            THEN( "the filter first returns coefficient 0 times inmpulse" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == -10.0);
                REQUIRE( filter_data.current_value == output);
            }

            output=0.0;
            status = dh_filter(&filter_data, 0.0F, &output);
            THEN( "the filter first returns coefficient 1 times last input" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 5.0);
                REQUIRE( filter_data.current_value == output);
            }
            
            output=0.0;
            status = dh_filter(&filter_data, 0.0F, &output);
            THEN( "the filter first returns coefficient 2 times first input" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == -15.0);
                REQUIRE( filter_data.current_value == output);
            }
            
            output=0.0;
            status = dh_filter(&filter_data, 0.0F, &output);
            THEN( "the filter first returns coefficient 3 times first input" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == -30.0);
                REQUIRE( filter_data.current_value == output);
            }
            
            output=0.0;
            status = dh_filter(&filter_data, 0.0F, &output);
            THEN( "the filter first returns zero" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 0.0);
                REQUIRE( filter_data.current_value == output);
            }
        }
    }

    GIVEN( "A filter struct is manually created with different output parameters and the last output is set to 1" ) {
        dh_filter_data filter_data;
        filter_data.initialized = true;
        filter_data.current_input_index = 0;
        filter_data.number_coefficients_in = 1;
        filter_data.number_coefficients_out = 4;
        filter_data.current_output_index = 0;
        DH_FILTER_VALUE_TYPE indata[1] = {0.0};
        DH_FILTER_VALUE_TYPE incoeffs[1] = {0.0};
        DH_FILTER_VALUE_TYPE outdata[4] = {1.0, 0.0, 0.0, 0.0};
        DH_FILTER_VALUE_TYPE outcoeffs[4] = {1.0, -2.0, -3.0, -4.0};
        filter_data.inputs = indata;
        filter_data.coefficients_in = incoeffs;
        filter_data.outputs = outdata;
        filter_data.coefficients_out = outcoeffs;

        WHEN( "filter is called" ) {  
            DH_FILTER_VALUE_TYPE output=0.0;
            int status = dh_filter(&filter_data, 0.0F, &output);

            THEN( "the filter returns -(output coefficient 1) times last output" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 2.0);
                REQUIRE( filter_data.current_value == output);
            }

            output=0.0;
            status = dh_filter(&filter_data, 0.0F, &output);
            THEN( "the filter return value computed from weigthed sum of last two outputs" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 7.0);
                REQUIRE( filter_data.current_value == output);
            }

            output=0.0;
            status = dh_filter(&filter_data, 0.0F, &output);
            THEN( "the filter return value computed from weigthed sum of last three outputs" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 24.0);
                REQUIRE( filter_data.current_value == output);
            }

            output=0.0;
            status = dh_filter(&filter_data, 0.0F, &output);
            THEN( "the filter return value computed from weigthed sum of last three outputs" ) {
                REQUIRE( status == DH_FILTER_OK);
                REQUIRE( output == 77.0);
                REQUIRE( filter_data.current_value == output);
            }
        }
    }
}
