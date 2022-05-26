
#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"
#include "dh/filter.h"
#include "cpp/filter.hpp"
#include <cstring>

/**
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

SCENARIO( "The cpp bindings can be used", "[filter]" ) {

    GIVEN( "An options structure filled with values for FIR exponential lowpass filters" ) {
        dh_filter_parameters opts;
        opts.filter_type = DH_FIR_EXPONENTIAL_MOVING_AVERAGE_LOWPASS;
        opts.cutoff_frequency_low = 15;
        opts.cutoff_frequency_high = 25;
        opts.sampling_frequency = 100;
        opts.filter_order = 4;
        opts.ripple = -3.0;
        
        WHEN( "the create function is called" ) {
            auto filt = dh::filter(opts);
            auto resp = filt.compute_frequency_response(200);
            THEN( "response can be computed" ) {
                REQUIRE(resp.size() == 201);
                REQUIRE(filt.feedback_coefficients().size() == 1);
                REQUIRE(filt.feedforward_coefficients().size() == 5);
                for(auto & sp: filt.feedforward_coefficients()){
                    printf("%f\n",sp);
                }
            }
        }
    }

    GIVEN( "An options structure filled with values for FIR moving average lowpass filters" ) {
        dh_filter_parameters opts;
        opts.filter_type = DH_FIR_MOVING_AVERAGE_LOWPASS;
        opts.cutoff_frequency_low = 15;
        opts.cutoff_frequency_high = 25;
        opts.sampling_frequency = 100;
        opts.filter_order = 4;
        opts.ripple = -3.0;
        
        WHEN( "the create function is called" ) {
            auto filt = dh::filter(opts);
            auto resp = filt.compute_frequency_response(200);
            THEN( "response can be computed" ) {
                REQUIRE(resp.size() == 201);
                REQUIRE(filt.good() == true);
                REQUIRE(filt.feedback_coefficients().size() == 1);
                REQUIRE(filt.feedforward_coefficients().size() == 5);
            }
        }
    }

    
    GIVEN( "An options structure filled with values for FIR moving average highpass filters" ) {
        dh_filter_parameters opts;
        opts.filter_type = DH_FIR_MOVING_AVERAGE_HIGHPASS;
        opts.cutoff_frequency_low = 15;
        opts.cutoff_frequency_high = 25;
        opts.sampling_frequency = 100;
        opts.filter_order = 4;
        opts.ripple = -3.0;
        
        WHEN( "the create function is called" ) {
            auto filt = dh::filter(opts);
            auto resp = filt.compute_frequency_response(200);
            THEN( "response can be computed" ) {
                REQUIRE(resp.size() == 201);
            }
        }
    }
}





