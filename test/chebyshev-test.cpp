#include "catch2/catch_test_macros.hpp"
#include "dh/chebyshev.h"
#include "catch2/catch_approx.hpp"


/**
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

SCENARIO("Chebyshev output coefficients can be computed", "[filter]") {
     GIVEN("parameters: fourth order chebyshev, 100 Hz sampling rate, 25Hz cutoff, 3 db ripple, lowpass") {
          WHEN("denominator is computed") {
               double coeffs[5];
               double transformed = transform_frequency(0.25);
               int status = compute_chebyshev_lowpass_denominator(coeffs, 4, transformed, -3.0);
               THEN("results are correct") {
                    REQUIRE(status == DH_FILTER_OK);
                    REQUIRE(coeffs[4] == Catch::Approx(0.4080348558));
                    REQUIRE(coeffs[3] == Catch::Approx(-0.8817615727));
                    REQUIRE(coeffs[2] == Catch::Approx(1.4174806064));
                    REQUIRE(coeffs[1] == Catch::Approx(-1.0939919582));
                    REQUIRE(coeffs[0] == Catch::Approx(1.0));
               }
          }
     }

     GIVEN("parameters: fifth order chebyshev, 100 Hz sampling rate, 20Hz cutoff, 2 db ripple, high pass") {
          WHEN("coefficients are computed") {
               double num[6];
               double denom[6];
               int status = compute_chebyshev_highpass_coefficients(num,denom,5,20,100, -2.0);
               THEN("results are correct") {
                    REQUIRE(status == DH_FILTER_OK);
                    REQUIRE(denom[5] == Catch::Approx(0.2657574384));
                    REQUIRE(denom[4] == Catch::Approx(0.4386539929));
                    REQUIRE(denom[3] == Catch::Approx(0.6053373580));
                    REQUIRE(denom[2] == Catch::Approx(1.1506939843));
                    REQUIRE(denom[1] == Catch::Approx(0.2289297075));
                    REQUIRE(denom[0] == Catch::Approx(1.0));
               }
          }
     }
     
     GIVEN("parameters: third order chebyshev, 100 Hz sampling rate, 15, 30Hz cutoff, 3 db ripple, band pass") {
          WHEN("coefficients are computed") {
               double num[7];
               double denom[7];
               int status = compute_chebyshev_bandfilter_coefficients(num,denom,3,15,30,100, true, -3.0);
               THEN("results are correct") {
                    REQUIRE(status == DH_FILTER_OK);
                    REQUIRE(denom[6] == Catch::Approx( 0.5724244477));
                    REQUIRE(denom[5] == Catch::Approx(-0.5855928827));
                    REQUIRE(denom[4] == Catch::Approx( 1.8286681732));
                    REQUIRE(denom[3] == Catch::Approx(-1.2549361046));
                    REQUIRE(denom[2] == Catch::Approx( 2.1357801602));
                    REQUIRE(denom[1] == Catch::Approx(-0.8563748024));
                    REQUIRE(denom[0] == Catch::Approx(1.0));

                    double gain = 4.731343700e+001;
                    REQUIRE(num[6] == Catch::Approx(-1.0/gain));
                    REQUIRE(num[5] == Catch::Approx(0.0));
                    REQUIRE(num[4] == Catch::Approx(3.0/gain));
                    REQUIRE(num[3] == Catch::Approx(0.0));
                    REQUIRE(num[2] == Catch::Approx(-3.0/gain));
                    REQUIRE(num[1] == Catch::Approx(0.0));
                    REQUIRE(num[0] == Catch::Approx(1.0/gain));
               }
          }
     }

     GIVEN("parameters: third order chebyshev, 100 Hz sampling rate, 15, 30Hz cutoff, 3 db ripple, band stop") {
          WHEN("coefficients are computed") {
               double num[7];
               double denom[7];
               int status = compute_chebyshev_bandfilter_coefficients(num,denom,3,15,30,100, false, -3.0);
               THEN("results are correct") {
                    REQUIRE(status == DH_FILTER_OK);
                    REQUIRE(denom[6] == Catch::Approx(-0.1974887770));
                    REQUIRE(denom[5] == Catch::Approx( 0.0136148879));
                    REQUIRE(denom[4] == Catch::Approx( 0.5489194787));
                    REQUIRE(denom[3] == Catch::Approx(-0.4252673736));
                    REQUIRE(denom[2] == Catch::Approx( 0.8150006537));
                    REQUIRE(denom[1] == Catch::Approx(-0.6435743840));
                    REQUIRE(denom[0] == Catch::Approx(1.0));

                    double gain = 4.034192003e+000;
                    REQUIRE(num[6] == Catch::Approx(1.0/gain));
                    REQUIRE(num[5] == Catch::Approx(-1.0534230275/gain));
                    REQUIRE(num[4] == Catch::Approx(3.3699000250/gain));
                    REQUIRE(num[3] == Catch::Approx(-2.1501417444/gain));
                    REQUIRE(num[2] == Catch::Approx(3.3699000250/gain));
                    REQUIRE(num[1] == Catch::Approx(-1.0534230275/gain));
                    REQUIRE(num[0] == Catch::Approx(1.0/gain));
               }
          }
     }
}
