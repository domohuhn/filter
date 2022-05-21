#include "catch2/catch_test_macros.hpp"
#include "dh/filter.h"
#include "catch2/catch_approx.hpp"

/**
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

SCENARIO( "Brickwall lowpass filters can be initialized", "[filter]" ) {
    GIVEN("The filter parameters for a 10th order brickwall lowpass filter with 100 Hz sampling rate and 25Hz cutoff"){
        dh_filter_data filter_data;
        dh_filter_parameters opts;
        opts.filter_type = DH_FIR_BRICKWALL_LOWPASS;
        opts.cutoff_frequency_low = 25.0;
        opts.sampling_frequency = 100.0;
        opts.filter_order = 10;
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
        dh_filter_parameters opts;
        opts.filter_type = DH_FIR_BRICKWALL_HIGHPASS;
        opts.cutoff_frequency_low = 25.0;
        opts.sampling_frequency = 100.0;
        opts.filter_order = 10;
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


SCENARIO( "Brickwall bandstop filters can be initialized", "[filter]" ) {
    GIVEN("The filter parameters for a 10th order bandstop highpass filter with 100 Hz sampling rate and 20Hz-40Hz cutoff"){
        dh_filter_data filter_data;
        dh_filter_parameters opts;
        opts.filter_type = DH_FIR_BRICKWALL_BANDSTOP;
        opts.cutoff_frequency_low = 20.0;
        opts.cutoff_frequency_high = 40.0;
        opts.sampling_frequency = 100.0;
        opts.filter_order = 10;
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
            THEN( "The input array is zeroed" ) {
                REQUIRE(filter_data.inputs[0] == 0.0);
                REQUIRE(filter_data.inputs[1] == 0.0);
                REQUIRE(filter_data.inputs[2] == 0.0);
                REQUIRE(filter_data.inputs[3] == 0.0);
                REQUIRE(filter_data.inputs[4] == 0.0);
                REQUIRE(filter_data.inputs[5] == 0.0);
                REQUIRE(filter_data.inputs[6] == 0.0);
                REQUIRE(filter_data.inputs[7] == 0.0);
                REQUIRE(filter_data.inputs[8] == 0.0);
                REQUIRE(filter_data.inputs[9] == 0.0);
                REQUIRE(filter_data.inputs[10] == 0.0);
            }
            THEN( "The output array is zeroed" ) {
                REQUIRE(filter_data.outputs[0] == 0.0);
            }
            THEN( "The coefficients are correct" ) {
                REQUIRE( filter_data.coefficients_in[0]  == Catch::Approx(1.48158749279202e-017   ) );
                REQUIRE( filter_data.coefficients_in[1]  == Catch::Approx(-0.0348394758192576     ) );
                REQUIRE( filter_data.coefficients_in[2]  == Catch::Approx(-0.171048089807929      ) );
                REQUIRE( filter_data.coefficients_in[3]  == Catch::Approx(0.256572134711894       ) );
                REQUIRE( filter_data.coefficients_in[4]  == Catch::Approx(0.13935790327703        ) );
                REQUIRE( filter_data.coefficients_in[5]  == Catch::Approx(0.619915055276525) );
                REQUIRE( filter_data.coefficients_in[6]  == Catch::Approx(0.13935790327703) );
                REQUIRE( filter_data.coefficients_in[7]  == Catch::Approx(0.256572134711894) );
                REQUIRE( filter_data.coefficients_in[8]  == Catch::Approx(-0.171048089807929) );
                REQUIRE( filter_data.coefficients_in[9]  == Catch::Approx(-0.0348394758192576) );
                REQUIRE( filter_data.coefficients_in[10] == Catch::Approx(1.48158749279202e-017) );

                REQUIRE( filter_data.coefficients_out[0] == Catch::Approx(1.0) );
            }
            REQUIRE(dh_free_filter(&filter_data) == DH_FILTER_OK);
        }
    }
}


SCENARIO( "Brickwall bandpass filters can be initialized", "[filter]" ) {
    GIVEN("The filter parameters for a 10th order brickwall bandpass filter with 100 Hz sampling rate and 20Hz-40Hz cutoff"){
        dh_filter_data filter_data;
        dh_filter_parameters opts;
        opts.filter_type = DH_FIR_BRICKWALL_BANDPASS;
        opts.cutoff_frequency_low = 20.0;
        opts.cutoff_frequency_high = 40.0;
        opts.sampling_frequency = 100.0;
        opts.filter_order = 10;
        WHEN( "The filter is created" ) {
            int status = dh_create_filter(&filter_data,&opts);
            THEN( "The status is ok" ) {
                REQUIRE( status == DH_FILTER_OK );
                REQUIRE(filter_data.coefficients_in != (void*)NULL);
                REQUIRE(filter_data.coefficients_out != (void*)NULL);
                REQUIRE(filter_data.initialized == true);
                REQUIRE(filter_data.buffer_needs_cleanup == true);
                REQUIRE(filter_data.number_coefficients_in == 21);
                REQUIRE(filter_data.number_coefficients_out == 1);
            }
            THEN( "The input array is zeroed" ) {
                REQUIRE(filter_data.inputs[0] == 0.0);
                REQUIRE(filter_data.inputs[1] == 0.0);
                REQUIRE(filter_data.inputs[2] == 0.0);
                REQUIRE(filter_data.inputs[3] == 0.0);
                REQUIRE(filter_data.inputs[4] == 0.0);
                REQUIRE(filter_data.inputs[5] == 0.0);
                REQUIRE(filter_data.inputs[6] == 0.0);
                REQUIRE(filter_data.inputs[7] == 0.0);
                REQUIRE(filter_data.inputs[8] == 0.0);
                REQUIRE(filter_data.inputs[9] == 0.0);
                REQUIRE(filter_data.inputs[10] == 0.0);
                REQUIRE(filter_data.inputs[11] == 0.0);
                REQUIRE(filter_data.inputs[12] == 0.0);
                REQUIRE(filter_data.inputs[13] == 0.0);
                REQUIRE(filter_data.inputs[14] == 0.0);
                REQUIRE(filter_data.inputs[15] == 0.0);
                REQUIRE(filter_data.inputs[16] == 0.0);
                REQUIRE(filter_data.inputs[17] == 0.0);
                REQUIRE(filter_data.inputs[18] == 0.0);
                REQUIRE(filter_data.inputs[19] == 0.0);
                REQUIRE(filter_data.inputs[20] == 0.0);
            }
            THEN( "The output array is zeroed" ) {
                REQUIRE(filter_data.outputs[0] == 0.0);
            }
            THEN( "The coefficients are correct" ) {
                REQUIRE( filter_data.coefficients_in[0]  == Catch::Approx(-5.41526208764961e-034   ) );
                REQUIRE( filter_data.coefficients_in[1]  == Catch::Approx(-3.44076173491982e-018) );
                REQUIRE( filter_data.coefficients_in[2]  == Catch::Approx( -0.00394258906877782) );
                REQUIRE( filter_data.coefficients_in[3]  == Catch::Approx(0.0052567854250371) );
                REQUIRE( filter_data.coefficients_in[4]  == Catch::Approx(-0.000876130904172841) );
                REQUIRE( filter_data.coefficients_in[5]  == Catch::Approx(0.0105135708500742) );
                REQUIRE( filter_data.coefficients_in[6]  == Catch::Approx(0.0352702861423298) );
                REQUIRE( filter_data.coefficients_in[7]  == Catch::Approx(0.176691011060148) );
                REQUIRE( filter_data.coefficients_in[8]  == Catch::Approx(-0.262408123877704) );
                REQUIRE( filter_data.coefficients_in[9]  == Catch::Approx(-0.146337929994356) );
                REQUIRE( filter_data.coefficients_in[10] == Catch::Approx(0.371666240734843) );
                REQUIRE( filter_data.coefficients_in[11] == Catch::Approx(-0.146337929994356) );
                REQUIRE( filter_data.coefficients_in[12] == Catch::Approx(-0.262408123877704) );
                REQUIRE( filter_data.coefficients_in[13] == Catch::Approx(0.176691011060148) );
                REQUIRE( filter_data.coefficients_in[14] == Catch::Approx(0.0352702861423298) );
                REQUIRE( filter_data.coefficients_in[15] == Catch::Approx(0.0105135708500742) );
                REQUIRE( filter_data.coefficients_in[16] == Catch::Approx(-0.000876130904172842) );
                REQUIRE( filter_data.coefficients_in[17] == Catch::Approx(0.0052567854250371) );
                REQUIRE( filter_data.coefficients_in[18] == Catch::Approx(-0.00394258906877782) );
                REQUIRE( filter_data.coefficients_in[19] == Catch::Approx(-3.44076173491982e-018) );
                REQUIRE( filter_data.coefficients_in[20] == Catch::Approx(-5.41526208764961e-034) );

                REQUIRE( filter_data.coefficients_out[0] == Catch::Approx(1.0) );
            }
            REQUIRE(dh_free_filter(&filter_data) == DH_FILTER_OK);
        }
    }
}



















