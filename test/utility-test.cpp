#include "catch2/catch_test_macros.hpp"
#include "dh/utility.h"
#include "catch2/catch_approx.hpp"
#include "complex_bridge.h"

/**
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

SCENARIO( "S-plane pole coefficients can be computed", "[filter]" ) {
    GIVEN( "An array of size 3" ) {
        COMPLEX arr[3];
        double real[3];
        double imag[3];
        WHEN( "the s-plane poles are computed" ) {
            dh_compute_poles_on_s_plane(arr,3);
            split_complex_number_arrays(arr,real,imag,3);
            THEN( "results are correct" ) {
                
                REQUIRE(real[0] == Catch::Approx(-0.5));
                REQUIRE(real[1] == Catch::Approx(-1.0));
                REQUIRE(real[2] == Catch::Approx(-0.5));

                REQUIRE(imag[0] == Catch::Approx(sqrt(3.0)/2.0));
                REQUIRE(imag[1] == Catch::Approx(0.0).margin(0.0001));
                REQUIRE(imag[2] == Catch::Approx(-sqrt(3.0)/2.0));
            }
        }
    }
    GIVEN( "An array of size 4" ) {
        COMPLEX arr[4];
        double real[4];
        double imag[4];
        WHEN( "the s-plane poles are computed" ) {
            dh_compute_poles_on_s_plane(arr,4);
            split_complex_number_arrays(arr,real,imag,4);
            THEN( "results are correct" ) {
                REQUIRE(real[0] == Catch::Approx(-0.382683));
                REQUIRE(real[1] == Catch::Approx(-0.923880));
                REQUIRE(real[2] == Catch::Approx(-0.923880));
                REQUIRE(real[3] == Catch::Approx(-0.382683));

                REQUIRE(imag[0] == Catch::Approx(0.923880));
                REQUIRE(imag[1] == Catch::Approx(0.382683));
                REQUIRE(imag[2] == Catch::Approx(-0.382683));
                REQUIRE(imag[3] == Catch::Approx(-0.923880));
            }
        }
    }
}


SCENARIO( "Convolution of parameters", "[filter]" ) {
    GIVEN( "Two Arrays of size 3" ) {
        double filt1[] = {1.0,2.0,3.0};
        double filt2[] = {-4.0,-5.0,1.0};
        WHEN( "the convolution is computed" ) {
            double out[5] = {};
            dh_convolve_parameters(filt1,filt2,3,out);
            THEN( "the result is correct" ) {
                REQUIRE(out[0] == filt1[0]*filt2[0]);
                REQUIRE(out[1] == filt1[0]*filt2[1] + filt1[1]*filt2[0]);
                REQUIRE(out[2] == filt1[0]*filt2[2] + filt1[1]*filt2[1] + filt1[2]*filt2[0]);
                REQUIRE(out[3] == filt1[2]*filt2[1] + filt1[1]*filt2[2]);
                REQUIRE(out[4] == filt1[2]*filt2[2]);
            }
        }
    }
}

