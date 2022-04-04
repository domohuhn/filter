#include "catch2/catch_test_macros.hpp"
#include "dh/butterworth.h"
#include "dh/filter.h"
#include "catch2/catch_approx.hpp"

/**
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

SCENARIO( "Butterworth output coefficients can be computed", "[filter]" ) {
    GIVEN("parameters: fifth order butterworth, 100 Hz sampling rate, 20Hz cutoff"){
        WHEN( "denominator is computed" ) {
            double coeffs[6];
            double transformed = transform_frequency(0.2);
            int status = compute_butterworth_lowpass_denominator(coeffs,5,transformed);
            THEN( "results are correct" ) {
                REQUIRE( status == DH_FILTER_OK );
                REQUIRE( coeffs[0] == Catch::Approx(1.0) );
                REQUIRE( coeffs[1] == Catch::Approx(-0.9853252393) );
                REQUIRE( coeffs[2] == Catch::Approx(0.9738493318) );
                REQUIRE( coeffs[3] == Catch::Approx(-0.3863565586)  );
                REQUIRE( coeffs[4] == Catch::Approx(0.1111638406) );
                REQUIRE( coeffs[5] == Catch::Approx(-0.0112635125) );
            }
        }
    }

    GIVEN("parameters: fifth order butterworth, 100 Hz sampling rate, 25Hz cutoff"){
        WHEN( "lowpass parameters are computed" ) {
            double num[6];
            double denom[6];
            int status = compute_butterworth_lowpass_coefficients(num,denom,5,25,100);
            
            THEN( "status is ok" ) {
                REQUIRE( status == DH_FILTER_OK );
            }
            THEN( "results are correct" ) {
                REQUIRE( denom[0] == Catch::Approx(1.0) );
                REQUIRE( denom[1] == Catch::Approx(-0.0).margin(1e-9) );
                REQUIRE( denom[2] == Catch::Approx(0.6334368540) );
                REQUIRE( denom[3] == Catch::Approx(-0.0).margin(1e-9)  );
                REQUIRE( denom[4] == Catch::Approx(0.0557280900) );
                REQUIRE( denom[5] == Catch::Approx(-0.0).margin(1e-9) );

                double gain = 1.894427191e+001;
                REQUIRE( num[0] == Catch::Approx(1.0/gain) );
                REQUIRE( num[1] == Catch::Approx(5.0/gain) );
                REQUIRE( num[2] == Catch::Approx(10.0/gain) );
                REQUIRE( num[3] == Catch::Approx(10.0/gain)  );
                REQUIRE( num[4] == Catch::Approx(5.0/gain) );
                REQUIRE( num[5] == Catch::Approx(1.0/gain) );

            }
        }
    }

    
    GIVEN("parameters: fifth order butterworth, 100 Hz sampling rate, 25Hz cutoff"){
        WHEN( "highpass parameters are computed" ) {
            double num[6];
            double denom[6];
            int status = compute_butterworth_highpass_coefficients(num,denom,5,25,100);
            
            THEN( "status is ok" ) {
                REQUIRE( status == DH_FILTER_OK );
            }
            THEN( "results are correct" ) {
                REQUIRE( denom[0] == Catch::Approx(1.0) );
                REQUIRE( denom[1] == Catch::Approx(-0.0).margin(1e-9) );
                REQUIRE( denom[2] == Catch::Approx(0.6334368540) );
                REQUIRE( denom[3] == Catch::Approx(-0.0).margin(1e-9)  );
                REQUIRE( denom[4] == Catch::Approx(0.0557280900) );
                REQUIRE( denom[5] == Catch::Approx(-0.0).margin(1e-9) );

                double gain = 1.894427191e+001;
                REQUIRE( num[0] == Catch::Approx(1.0/gain) );
                REQUIRE( num[1] == Catch::Approx(-5.0/gain) );
                REQUIRE( num[2] == Catch::Approx(10.0/gain) );
                REQUIRE( num[3] == Catch::Approx(-10.0/gain)  );
                REQUIRE( num[4] == Catch::Approx(5.0/gain) );
                REQUIRE( num[5] == Catch::Approx(-1.0/gain) );
            }
        }
    }

    GIVEN("parameters: third order butterworth, 100 Hz sampling rate, 15-30Hz bandstop"){
        WHEN( "bandstop parameters are computed" ) {
            double num[7];
            double denom[7];
            int status = compute_butterworth_bandfilter_coefficients(num,denom,3,15,30,100,false);
            
            THEN( "status is ok" ) {
                REQUIRE( status == DH_FILTER_OK );
            }
            THEN( "results are correct" ) {
                REQUIRE( denom[0] == Catch::Approx(1.0) );
                REQUIRE( denom[1] == Catch::Approx(-0.7307099526).margin(1e-9) );
                REQUIRE( denom[2] == Catch::Approx(1.3474771445) );
                REQUIRE( denom[3] == Catch::Approx(-0.6685831068).margin(1e-9)  );
                REQUIRE( denom[4] == Catch::Approx(0.7874032156) );
                REQUIRE( denom[5] == Catch::Approx(-0.1947474843).margin(1e-9) );
                REQUIRE( denom[6] == Catch::Approx(0.1377613013).margin(1e-9) );

                double gain = 2.670564319;
                REQUIRE( num[0] == Catch::Approx(1.0/gain) );
                REQUIRE( num[1] == Catch::Approx(-1.0534230275/gain) );
                REQUIRE( num[2] == Catch::Approx(3.3699000250/gain) );
                REQUIRE( num[3] == Catch::Approx(-2.1501417444/gain)  );
                REQUIRE( num[4] == Catch::Approx(3.3699000250/gain) );
                REQUIRE( num[5] == Catch::Approx(-1.0534230275/gain) );
                REQUIRE( num[6] == Catch::Approx(1.0/gain) );
            }
        }
    }

    GIVEN("parameters: third order butterworth, 100 Hz sampling rate, 15-30Hz bandpass"){
        WHEN( "bandpass parameters are computed" ) {
            double num[7];
            double denom[7];
            int status = compute_butterworth_bandfilter_coefficients(num,denom,3,15,30,100,true);
            
            THEN( "status is ok" ) {
                REQUIRE( status == DH_FILTER_OK );
            }
            THEN( "results are correct" ) {
                REQUIRE( denom[0] == Catch::Approx(1.0) );
                REQUIRE( denom[1] == Catch::Approx(-0.7307099526).margin(1e-9) );
                REQUIRE( denom[2] == Catch::Approx(1.3474771445) );
                REQUIRE( denom[3] == Catch::Approx(-0.6685831068).margin(1e-9)  );
                REQUIRE( denom[4] == Catch::Approx(0.7874032156) );
                REQUIRE( denom[5] == Catch::Approx(-0.1947474843).margin(1e-9) );
                REQUIRE( denom[6] == Catch::Approx(0.1377613013).margin(1e-9) );

                double gain = 2.018856262e+001;
                REQUIRE( num[0] == Catch::Approx(1.0/gain) );
                REQUIRE( num[1] == Catch::Approx(0.0) );
                REQUIRE( num[2] == Catch::Approx(-3.0/gain) );
                REQUIRE( num[3] == Catch::Approx(0.0)  );
                REQUIRE( num[4] == Catch::Approx(3.0/gain) );
                REQUIRE( num[5] == Catch::Approx(0.0) );
                REQUIRE( num[6] == Catch::Approx(-1.0/gain) );
            }
        }
    }
}

SCENARIO( "Butterworth lowpass filters can be initialized", "[filter]" ) {
    GIVEN("parameters: fifth order butterworth, 100 Hz sampling rate, 25Hz cutoff"){
        dh_filter_data filter_data;
        dh_filter_options opts;
        opts.filter_type = DH_IIR_BUTTERWORTH_LOWPASS;
        opts.parameters.butterworth.cutoff_frequency_hz = 25.0;
        opts.parameters.butterworth.sampling_frequency_hz = 100.0;
        opts.parameters.butterworth.filter_order = 5;
        WHEN( "lowpass parameters are computed" ) {
            int status = dh_create_filter(&filter_data,&opts);
            THEN( "status is ok" ) {
                REQUIRE( status == DH_FILTER_OK );
                REQUIRE(filter_data.coefficients_in != (void*)NULL);
                REQUIRE(filter_data.coefficients_out != (void*)NULL);
                REQUIRE(filter_data.initialized == false);
                REQUIRE(filter_data.buffer_needs_cleanup == true);
                REQUIRE(filter_data.number_coefficients_in == 6);
                REQUIRE(filter_data.number_coefficients_out == 6);
            }
            THEN( "coefficients are correct" ) {
                REQUIRE( filter_data.coefficients_out[0] == Catch::Approx(1.0) );
                REQUIRE( filter_data.coefficients_out[1] == Catch::Approx(-0.0).margin(1e-9) );
                REQUIRE( filter_data.coefficients_out[2] == Catch::Approx(0.6334368540) );
                REQUIRE( filter_data.coefficients_out[3] == Catch::Approx(-0.0).margin(1e-9)  );
                REQUIRE( filter_data.coefficients_out[4] == Catch::Approx(0.0557280900) );
                REQUIRE( filter_data.coefficients_out[5] == Catch::Approx(-0.0).margin(1e-9) );

                double gain = 1.894427191e+001;
                REQUIRE( filter_data.coefficients_in[0] == Catch::Approx(1.0/gain) );
                REQUIRE( filter_data.coefficients_in[1] == Catch::Approx(5.0/gain) );
                REQUIRE( filter_data.coefficients_in[2] == Catch::Approx(10.0/gain) );
                REQUIRE( filter_data.coefficients_in[3] == Catch::Approx(10.0/gain)  );
                REQUIRE( filter_data.coefficients_in[4] == Catch::Approx(5.0/gain) );
                REQUIRE( filter_data.coefficients_in[5] == Catch::Approx(1.0/gain) );
            }
            REQUIRE(dh_free_filter(&filter_data) == DH_FILTER_OK);
        }
    }
}


SCENARIO( "Butterworth highpass filters can be initialized", "[filter]" ) {   
    GIVEN("parameters: fifth order butterworth, 100 Hz sampling rate, 25Hz cutoff"){
        dh_filter_data filter_data;
        dh_filter_options opts;
        opts.filter_type = DH_IIR_BUTTERWORTH_HIGHPASS;
        opts.parameters.butterworth.cutoff_frequency_hz = 25.0;
        opts.parameters.butterworth.sampling_frequency_hz = 100.0;
        opts.parameters.butterworth.filter_order = 5;
        WHEN( "highpass parameters are computed" ) {
            int status = dh_create_filter(&filter_data,&opts);
            THEN( "status is ok" ) {
                REQUIRE( status == DH_FILTER_OK );
                REQUIRE(filter_data.coefficients_in != (void*)NULL);
                REQUIRE(filter_data.coefficients_out != (void*)NULL);
                REQUIRE(filter_data.initialized == true);
                REQUIRE(filter_data.buffer_needs_cleanup == true);
                REQUIRE(filter_data.number_coefficients_in == 6);
                REQUIRE(filter_data.number_coefficients_out == 6);
            }
            THEN( "coefficients are correct" ) {
                REQUIRE( filter_data.coefficients_out[0] == Catch::Approx(1.0) );
                REQUIRE( filter_data.coefficients_out[1] == Catch::Approx(-0.0).margin(1e-9) );
                REQUIRE( filter_data.coefficients_out[2] == Catch::Approx(0.6334368540) );
                REQUIRE( filter_data.coefficients_out[3] == Catch::Approx(-0.0).margin(1e-9)  );
                REQUIRE( filter_data.coefficients_out[4] == Catch::Approx(0.0557280900) );
                REQUIRE( filter_data.coefficients_out[5] == Catch::Approx(-0.0).margin(1e-9) );

                double gain = 1.894427191e+001;
                REQUIRE( filter_data.coefficients_in[0] == Catch::Approx(1.0/gain) );
                REQUIRE( filter_data.coefficients_in[1] == Catch::Approx(-5.0/gain) );
                REQUIRE( filter_data.coefficients_in[2] == Catch::Approx(10.0/gain) );
                REQUIRE( filter_data.coefficients_in[3] == Catch::Approx(-10.0/gain)  );
                REQUIRE( filter_data.coefficients_in[4] == Catch::Approx(5.0/gain) );
                REQUIRE( filter_data.coefficients_in[5] == Catch::Approx(-1.0/gain) );
            }
            REQUIRE(dh_free_filter(&filter_data) == DH_FILTER_OK);
        }
    }
}

SCENARIO( "Butterworth bandstop filters can be initialized", "[filter]" ) {
    GIVEN("parameters: third order butterworth, 100 Hz sampling rate, 15-30Hz bandstop"){
        dh_filter_data filter_data;
        dh_filter_options opts;
        opts.filter_type = DH_IIR_BUTTERWORTH_BANDSTOP;
        opts.parameters.butterworth.cutoff_frequency_hz = 15.0;
        opts.parameters.butterworth.cutoff_frequency_2_hz = 30.0;
        opts.parameters.butterworth.sampling_frequency_hz = 100.0;
        opts.parameters.butterworth.filter_order = 3;
        WHEN( "bandstop parameters are computed" ) {
            int status = dh_create_filter(&filter_data,&opts);
            THEN( "status is ok" ) {
                REQUIRE( status == DH_FILTER_OK );
                REQUIRE(filter_data.coefficients_in != (void*)NULL);
                REQUIRE(filter_data.coefficients_out != (void*)NULL);
                REQUIRE(filter_data.initialized == true);
                REQUIRE(filter_data.buffer_needs_cleanup == true);
                REQUIRE(filter_data.number_coefficients_in == 7);
                REQUIRE(filter_data.number_coefficients_out == 7);
            }
            THEN( "coefficients are correct" ) {
                REQUIRE( filter_data.coefficients_out[0] == Catch::Approx(1.0) );
                REQUIRE( filter_data.coefficients_out[1] == Catch::Approx(-0.7307099526).margin(1e-9) );
                REQUIRE( filter_data.coefficients_out[2] == Catch::Approx(1.3474771445) );
                REQUIRE( filter_data.coefficients_out[3] == Catch::Approx(-0.6685831068).margin(1e-9)  );
                REQUIRE( filter_data.coefficients_out[4] == Catch::Approx(0.7874032156) );
                REQUIRE( filter_data.coefficients_out[5] == Catch::Approx(-0.1947474843).margin(1e-9) );
                REQUIRE( filter_data.coefficients_out[6] == Catch::Approx(0.1377613013).margin(1e-9) );

                double gain = 2.670564319;
                REQUIRE( filter_data.coefficients_in[0] == Catch::Approx(1.0/gain) );
                REQUIRE( filter_data.coefficients_in[1] == Catch::Approx(-1.0534230275/gain) );
                REQUIRE( filter_data.coefficients_in[2] == Catch::Approx(3.3699000250/gain) );
                REQUIRE( filter_data.coefficients_in[3] == Catch::Approx(-2.1501417444/gain)  );
                REQUIRE( filter_data.coefficients_in[4] == Catch::Approx(3.3699000250/gain) );
                REQUIRE( filter_data.coefficients_in[5] == Catch::Approx(-1.0534230275/gain) );
                REQUIRE( filter_data.coefficients_in[6] == Catch::Approx(1.0/gain) );
            }
            REQUIRE(dh_free_filter(&filter_data) == DH_FILTER_OK);
        }
    }
}


SCENARIO( "Butterworth bandpass filters can be initialized", "[filter]" ) {
    GIVEN("parameters: third order butterworth, 100 Hz sampling rate, 15-30Hz bandpass"){
        dh_filter_data filter_data;
        dh_filter_options opts;
        opts.filter_type = DH_IIR_BUTTERWORTH_BANDPASS;
        opts.parameters.butterworth.cutoff_frequency_hz = 15.0;
        opts.parameters.butterworth.cutoff_frequency_2_hz = 30.0;
        opts.parameters.butterworth.sampling_frequency_hz = 100.0;
        opts.parameters.butterworth.filter_order = 3;
        WHEN( "bandpass parameters are computed" ) {
            int status = dh_create_filter(&filter_data,&opts);
            THEN( "status is ok" ) {
                REQUIRE( status == DH_FILTER_OK );
                REQUIRE(filter_data.coefficients_in != (void*)NULL);
                REQUIRE(filter_data.coefficients_out != (void*)NULL);
                REQUIRE(filter_data.initialized == true);
                REQUIRE(filter_data.buffer_needs_cleanup == true);
                REQUIRE(filter_data.number_coefficients_in == 7);
                REQUIRE(filter_data.number_coefficients_out == 7);
            }
            THEN( "results are correct" ) {
                REQUIRE( filter_data.coefficients_out[0] == Catch::Approx(1.0) );
                REQUIRE( filter_data.coefficients_out[1] == Catch::Approx(-0.7307099526).margin(1e-9) );
                REQUIRE( filter_data.coefficients_out[2] == Catch::Approx(1.3474771445) );
                REQUIRE( filter_data.coefficients_out[3] == Catch::Approx(-0.6685831068).margin(1e-9)  );
                REQUIRE( filter_data.coefficients_out[4] == Catch::Approx(0.7874032156) );
                REQUIRE( filter_data.coefficients_out[5] == Catch::Approx(-0.1947474843).margin(1e-9) );
                REQUIRE( filter_data.coefficients_out[6] == Catch::Approx(0.1377613013).margin(1e-9) );

                double gain = 2.018856262e+001;
                REQUIRE( filter_data.coefficients_in[0] == Catch::Approx(1.0/gain) );
                REQUIRE( filter_data.coefficients_in[1] == Catch::Approx(0.0) );
                REQUIRE( filter_data.coefficients_in[2] == Catch::Approx(-3.0/gain) );
                REQUIRE( filter_data.coefficients_in[3] == Catch::Approx(0.0)  );
                REQUIRE( filter_data.coefficients_in[4] == Catch::Approx(3.0/gain) );
                REQUIRE( filter_data.coefficients_in[5] == Catch::Approx(0.0) );
                REQUIRE( filter_data.coefficients_in[6] == Catch::Approx(-1.0/gain) );
            }
            REQUIRE(dh_free_filter(&filter_data) == DH_FILTER_OK);
        }
    }
}


