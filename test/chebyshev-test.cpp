#include "catch2/catch_test_macros.hpp"
#include "dh/chebyshev.h"
#include "catch2/catch_approx.hpp"


/**
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

SCENARIO("Chebyshev output coefficients can be computed", "[filter]") {
     GIVEN("parameters: fourth order chebyshev, 100 Hz sampling rate, 25Hz cutoff, 3 db ripple") {
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
}



/* high pass
20 / 100 order 5 ripple -2.0
gain 0.0

5: 0.2657574384
4: 0.4386539929
3: 0.6053373580
2: 1.1506939843
1: 0.2289297075
0: 1.0
*/


/* band pass
15, 30 / 100 order 3 ripple -3.0
gain ???

Recurrence relation:
y[n] = ( -1 * x[n- 6])
     + (  0 * x[n- 5])
     + (  3 * x[n- 4])
     + (  0 * x[n- 3])
     + ( -3 * x[n- 2])
     + (  0 * x[n- 1])
     + (  1 * x[n- 0])

     + ( -0.5724244477 * y[n- 6])
     + (  0.5855928827 * y[n- 5])
     + ( -1.8286681732 * y[n- 4])
     + (  1.2549361046 * y[n- 3])
     + ( -2.1357801602 * y[n- 2])
     + (  0.8563748024 * y[n- 1])

*/


/* band stop
15, 30/ 100 order 3 ripple -3.0
gain 4.034192003e+000

Recurrence relation:
y[n] = (  1 * x[n- 6])
     + ( -1.0534230275 * x[n- 5])
     + (  3.3699000250 * x[n- 4])
     + ( -2.1501417444 * x[n- 3])
     + (  3.3699000250 * x[n- 2])
     + ( -1.0534230275 * x[n- 1])
     + (  1 * x[n- 0])

     + (  0.1974887770 * y[n- 6])
     + ( -0.0136148879 * y[n- 5])
     + ( -0.5489194787 * y[n- 4])
     + (  0.4252673736 * y[n- 3])
     + ( -0.8150006537 * y[n- 2])
     + (  0.6435743840 * y[n- 1])


*/