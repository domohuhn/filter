#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include "dh/chebyshev.h"
#include "dh/filter.h"
#include "complex.h"



SCENARIO( "Chebyshev type 2 design", "[filter]" ) {
    GIVEN("I need to compute coefficients for a second order chebyshev type2 lowpass at 15 Hz/100Hz with -0.3dB ripple") {
      dh_filter_data filter_data;
      dh_filter_parameters opts;
      opts.filter_type = DH_IIR_CHEBYSHEV2_LOWPASS;
      opts.cutoff_frequency_low = 15.0;
      opts.sampling_frequency = 100.0;
      opts.ripple = -0.3;
      opts.filter_order = 2;
      WHEN("the values are computed"){
        int status = dh_create_filter(&filter_data,&opts);
        THEN("the status is ok"){
          REQUIRE(status == DH_FILTER_OK);
          REQUIRE(filter_data.coefficients_in != (void*)NULL);
          REQUIRE(filter_data.coefficients_out != (void*)NULL);
          REQUIRE(filter_data.initialized == false);
          REQUIRE(filter_data.buffer_needs_cleanup == true);
          REQUIRE(filter_data.number_coefficients_in == 3);
          REQUIRE(filter_data.number_coefficients_out == 3);
        }
        double * numerator = filter_data.coefficients_in;
        THEN("the feedforward coefficients are correct"){
          REQUIRE(numerator[0] == Catch::Approx(0.24838217728259915));
          REQUIRE(numerator[1] == Catch::Approx(-0.15720322174019702));
          REQUIRE(numerator[2] == Catch::Approx(0.24838217728259912));
        }
        double * denominator = filter_data.coefficients_out;
        THEN("the feedback coefficients are correct"){
          REQUIRE(denominator[0] == Catch::Approx(1.0));
          REQUIRE(denominator[1] == Catch::Approx(-1.0958722136716665));
          REQUIRE(denominator[2] == Catch::Approx(0.4354333464966677));
        }
        dh_free_filter(&filter_data);
      }
    }

    GIVEN("I need to compute coefficients for a second order chebyshev type2 highpass at 15 Hz/100Hz with -0.3dB ripple") {
      dh_filter_data filter_data;
      dh_filter_parameters opts;
      opts.filter_type = DH_IIR_CHEBYSHEV2_HIGHPASS;
      opts.cutoff_frequency_low = 15.0;
      opts.sampling_frequency = 100.0;
      opts.ripple = -0.3;
      opts.filter_order = 2;
      WHEN("the values are computed"){
        int status = dh_create_filter(&filter_data,&opts);
        THEN("the status is ok"){
          REQUIRE(status == DH_FILTER_OK);
          REQUIRE(filter_data.coefficients_in != (void*)NULL);
          REQUIRE(filter_data.coefficients_out != (void*)NULL);
          REQUIRE(filter_data.initialized == true);
          REQUIRE(filter_data.buffer_needs_cleanup == true);
          REQUIRE(filter_data.number_coefficients_in == 3);
          REQUIRE(filter_data.number_coefficients_out == 3);
        }
        double * numerator = filter_data.coefficients_in;
        THEN("the feedforward coefficients are correct"){
          REQUIRE(numerator[0] == Catch::Approx(0.47757040911141263));
          REQUIRE(numerator[1] == Catch::Approx(-0.7356610535644991));
          REQUIRE(numerator[2] == Catch::Approx(0.4775704091114126));
        }
        double * denominator = filter_data.coefficients_out;
        THEN("the feedback coefficients are correct"){
          REQUIRE(denominator[0] == Catch::Approx(1.0));
          REQUIRE(denominator[1] == Catch::Approx(-0.42063190394656946));
          REQUIRE(denominator[2] == Catch::Approx(0.27016996784075514));
        }
        dh_free_filter(&filter_data);
      }
    }

    GIVEN("I need to compute a second order bandpass at [30Hz,60Hz] for 200Hz sampling rate with 3db ripple") {
      dh_filter_data filter_data;
      dh_filter_parameters opts;
      opts.filter_type = DH_IIR_CHEBYSHEV2_BANDPASS;
      opts.cutoff_frequency_low = 30.0;
      opts.cutoff_frequency_high = 60.0;
      opts.sampling_frequency = 200.0;
      opts.ripple = -3.0;
      opts.filter_order = 2;
      WHEN("i compute the polynomial"){
        int status = dh_create_filter(&filter_data,&opts);
        THEN("the status is ok"){
          REQUIRE(status == DH_FILTER_OK);
        }
        double * num = filter_data.coefficients_in;
        double * denom = filter_data.coefficients_out;
        THEN("the feedforward coefficients are correct"){
          REQUIRE(num[0] == Catch::Approx(0.5860528708));
          REQUIRE(num[1] == Catch::Approx(-0.270909442));
          REQUIRE(num[2] == Catch::Approx(0.4184816227));
          REQUIRE(num[3] == Catch::Approx(-0.270909442));
          REQUIRE(num[4] == Catch::Approx(0.5860528708));
        }
        THEN("the feedback coefficients are correct"){
          REQUIRE(denom[0] == Catch::Approx(1.0));
          REQUIRE(denom[1] == Catch::Approx(-0.4725982399));
          REQUIRE(denom[2] == Catch::Approx(0.759131302));
          REQUIRE(denom[3] == Catch::Approx(-0.2945605108));
          REQUIRE(denom[4] == Catch::Approx(0.4929736931));
        }
        dh_free_filter(&filter_data);
      }
    }

    GIVEN("I need to compute a second order bandstop at [30Hz,60Hz] for 200Hz sampling rate with 3db ripple") {
      dh_filter_data filter_data;
      dh_filter_parameters opts;
      opts.filter_type = DH_IIR_CHEBYSHEV2_BANDSTOP;
      opts.cutoff_frequency_low = 30.0;
      opts.cutoff_frequency_high = 60.0;
      opts.sampling_frequency = 200.0;
      opts.ripple = -3.0;
      opts.filter_order = 2;
      WHEN("i compute the polynomial"){
        int status = dh_create_filter(&filter_data,&opts);
        THEN("the status is ok"){
          REQUIRE(status == DH_FILTER_OK);
        }
        double * num = filter_data.coefficients_in;
        double * denom = filter_data.coefficients_out;
        THEN("the feedforward coefficients are correct"){
          REQUIRE(num[0] == Catch::Approx(0.747036));
          REQUIRE(num[1] == Catch::Approx(-0.464353));
          REQUIRE(num[2] == Catch::Approx(1.232279));
          REQUIRE(num[3] == Catch::Approx(-0.464353));
          REQUIRE(num[4] == Catch::Approx(0.747036));
        }
        THEN("the feedback coefficients are correct"){
          REQUIRE(denom[0] == Catch::Approx(1.0));
          REQUIRE(denom[1] == Catch::Approx(-0.540645));
          REQUIRE(denom[2] == Catch::Approx(1.160887));
          REQUIRE(denom[3] == Catch::Approx(-0.388062));
          REQUIRE(denom[4] == Catch::Approx(0.565465));
        }
        dh_free_filter(&filter_data);
      }
    }
}
