#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include "dh/chebyshev.h"
#include "dh/filter.h"
#include "complex.h"



SCENARIO( "Chebyshev type 2 design", "[filter]" ) {
    GIVEN("I need to compute 4 zeros on the s-plane at 15 Hz/100Hz") {
      WHEN("i compute them"){
        auto f = std::tan (M_PI * 15.0/100.0);
        COMPLEX zeros[4];
        compute_zeros_on_s_plane_chebyshev2(zeros, 4, f);
        THEN("the result is correct"){
          REQUIRE(creal(zeros[0]) == Catch::Approx(0.0));
          REQUIRE(cimag(zeros[0]) == Catch::Approx(0.5515063724));
          REQUIRE(creal(zeros[1]) == Catch::Approx(0.0));
          REQUIRE(cimag(zeros[1]) == Catch::Approx(1.331454));
          REQUIRE(creal(zeros[2]) == Catch::Approx(0.0));
          REQUIRE(cimag(zeros[2]) == Catch::Approx(-1.331454));
          REQUIRE(creal(zeros[3]) == Catch::Approx(0.0));
          REQUIRE(cimag(zeros[3]) == Catch::Approx(-0.5515063724));
        }
      }
    }

    GIVEN("I need to compute coefficients for a second order chebyshev type2 lowpass at 15 Hz/100Hz with -0.3dB ripple") {
      double numerator[3];
      double denominator[3];
      size_t filter_order=2;
      double cutoff_hz = 15.0;
      double sampling_frequency_hz = 100.0;
      double ripple = -0.3;
      WHEN("the values are computed"){
        auto status = compute_chebyshev2_lowpass_coefficients(numerator,denominator,filter_order,cutoff_hz,sampling_frequency_hz,ripple);
        THEN("the status is ok"){
          REQUIRE(status == DH_FILTER_OK);
        }
        THEN("the feedforward coefficients are correct"){
          REQUIRE(numerator[0] == Catch::Approx(0.24838217728259915));
          REQUIRE(numerator[1] == Catch::Approx(-0.15720322174019702));
          REQUIRE(numerator[2] == Catch::Approx(0.24838217728259912));
        }
        THEN("the feedback coefficients are correct"){
          REQUIRE(denominator[0] == Catch::Approx(1.0));
          REQUIRE(denominator[1] == Catch::Approx(-1.0958722136716665));
          REQUIRE(denominator[2] == Catch::Approx(0.4354333464966677));
        }
      }
    }

    GIVEN("I need to compute coefficients for a second order chebyshev type2 highpass at 15 Hz/100Hz with -0.3dB ripple") {
      double numerator[3];
      double denominator[3];
      size_t filter_order=2;
      double cutoff_hz = 15.0;
      double sampling_frequency_hz = 100.0;
      double ripple = -0.3;
      WHEN("the values are computed"){
        auto status = compute_chebyshev2_highpass_coefficients(numerator,denominator,filter_order,cutoff_hz,sampling_frequency_hz,ripple);
        THEN("the status is ok"){
          REQUIRE(status == DH_FILTER_OK);
        }
        THEN("the feedforward coefficients are correct"){
          REQUIRE(numerator[0] == Catch::Approx(0.47757040911141263));
          REQUIRE(numerator[1] == Catch::Approx(-0.7356610535644991));
          REQUIRE(numerator[2] == Catch::Approx(0.4775704091114126));
        }
        THEN("the feedback coefficients are correct"){
          REQUIRE(denominator[0] == Catch::Approx(1.0));
          REQUIRE(denominator[1] == Catch::Approx(-0.42063190394656946));
          REQUIRE(denominator[2] == Catch::Approx(0.27016996784075514));
        }
      }
    }
}