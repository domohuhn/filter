#include "catch2/catch_test_macros.hpp"
#include "dh/filter.h"
#include "catch2/catch_approx.hpp"

/**
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

SCENARIO( "Brickwall lowpass filters can be initialized", "[filter]" ) {
    GIVEN("The filter parameters for a 10th order brickwall lowpass filter with 100 Hz sampling rate and 25Hz cutoff"){
        dh_filter_data filter_data;
        dh_filter_options opts;
        opts.filter_type = DH_FIR_BRICKWALL_LOWPASS;
        opts.parameters.brickwall.cutoff_frequency_hz = 25.0;
        opts.parameters.brickwall.sampling_frequency_hz = 100.0;
        opts.parameters.brickwall.filter_order = 10;
        WHEN( "The filter is created" ) {
            int status = dh_create_filter(&filter_data,&opts);
            THEN( "The status is ok" ) {
                REQUIRE( status == DH_FILTER_OK );
                REQUIRE(filter_data.coefficients_in != (void*)NULL);
                REQUIRE(filter_data.coefficients_out != (void*)NULL);
                REQUIRE(filter_data.initialized == false);
                REQUIRE(filter_data.buffer_needs_cleanup == true);
                REQUIRE(filter_data.number_coefficients_in == 11);
                REQUIRE(filter_data.number_coefficients_out == 1);
            }
            THEN( "The coefficients are correct" ) {
                REQUIRE( filter_data.coefficients_in[0]  == Catch::Approx( 0.0605303122372741   ) );
                REQUIRE( filter_data.coefficients_in[1]  == Catch::Approx(-1.85314512411546e-017) );
                REQUIRE( filter_data.coefficients_in[2]  == Catch::Approx(-0.10088385372879     ) );
                REQUIRE( filter_data.coefficients_in[3]  == Catch::Approx(1.85314512411546e-017 ) );
                REQUIRE( filter_data.coefficients_in[4]  == Catch::Approx(0.30265156118637      ) );
                REQUIRE( filter_data.coefficients_in[5]  == Catch::Approx(0.475403960610291) );
                REQUIRE( filter_data.coefficients_in[6]  == Catch::Approx(0.30265156118637) );
                REQUIRE( filter_data.coefficients_in[7]  == Catch::Approx(1.85314512411546e-017) );
                REQUIRE( filter_data.coefficients_in[8]  == Catch::Approx(-0.10088385372879) );
                REQUIRE( filter_data.coefficients_in[9]  == Catch::Approx(-1.85314512411546e-017) );
                REQUIRE( filter_data.coefficients_in[10] == Catch::Approx(0.0605303122372741) );

                REQUIRE( filter_data.coefficients_out[0] == Catch::Approx(1.0) );
            }
            REQUIRE(dh_free_filter(&filter_data) == DH_FILTER_OK);
        }
    }
}

SCENARIO( "Brickwall highpass filters can be initialized", "[filter]" ) {
    GIVEN("The filter parameters for a 10th order brickwall highpass filter with 100 Hz sampling rate and 25Hz cutoff"){
        dh_filter_data filter_data;
        dh_filter_options opts;
        opts.filter_type = DH_FIR_BRICKWALL_HIGHPASS;
        opts.parameters.brickwall.cutoff_frequency_hz = 25.0;
        opts.parameters.brickwall.sampling_frequency_hz = 100.0;
        opts.parameters.brickwall.filter_order = 10;
        WHEN( "The filter is created" ) {
            int status = dh_create_filter(&filter_data,&opts);
            THEN( "The status is ok" ) {
                REQUIRE( status == DH_FILTER_OK );
                REQUIRE(filter_data.coefficients_in != (void*)NULL);
                REQUIRE(filter_data.coefficients_out != (void*)NULL);
                REQUIRE(filter_data.initialized == true);
                REQUIRE(filter_data.buffer_needs_cleanup == true);
                REQUIRE(filter_data.number_coefficients_in == 11);
                REQUIRE(filter_data.number_coefficients_out == 1);
            }
            THEN( "The coefficients are correct" ) {
                REQUIRE( filter_data.coefficients_in[0]  == Catch::Approx(-0.0605303122372741   ) );
                REQUIRE( filter_data.coefficients_in[1]  == Catch::Approx(1.85314512411546e-017) );
                REQUIRE( filter_data.coefficients_in[2]  == Catch::Approx(0.10088385372879     ) );
                REQUIRE( filter_data.coefficients_in[3]  == Catch::Approx(-1.85314512411546e-017 ) );
                REQUIRE( filter_data.coefficients_in[4]  == Catch::Approx(-0.30265156118637      ) );
                REQUIRE( filter_data.coefficients_in[5]  == Catch::Approx(1.0-0.475403960610291) );
                REQUIRE( filter_data.coefficients_in[6]  == Catch::Approx(-0.30265156118637) );
                REQUIRE( filter_data.coefficients_in[7]  == Catch::Approx(-1.85314512411546e-017) );
                REQUIRE( filter_data.coefficients_in[8]  == Catch::Approx(0.10088385372879) );
                REQUIRE( filter_data.coefficients_in[9]  == Catch::Approx(1.85314512411546e-017) );
                REQUIRE( filter_data.coefficients_in[10] == Catch::Approx(-0.0605303122372741) );

                REQUIRE( filter_data.coefficients_out[0] == Catch::Approx(1.0) );
            }
            REQUIRE(dh_free_filter(&filter_data) == DH_FILTER_OK);
        }
    }
}
