#include "catch2/catch_test_macros.hpp"
#include "dh/utility.h"
#include "catch2/catch_approx.hpp"

/**
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

SCENARIO( "Transforms can be used", "[filter]" ) {
    GIVEN( "A complex number" ) {
        COMPLEX number = 2.0 + 1.0 * I;

        WHEN( "bilinear z transform is computed" ) {
            COMPLEX out = bilinear_z_transform(number);
            THEN( "the result is correct" ) {
                REQUIRE(creal(out) == -2.0);
                REQUIRE(cimag(out) == 1.0);
            }
        }
    }
}

SCENARIO( "S-plane pole coefficients can be computed", "[filter]" ) {
    GIVEN( "An array of size 3" ) {
        COMPLEX arr[3];
        WHEN( "s-plane poles are computed with scaling 1.0" ) {
            compute_poles_on_s_plane(arr,3,1.0);
            THEN( "results are correct" ) {
                
                REQUIRE(creal(arr[0]) == Catch::Approx(-0.5));
                REQUIRE(creal(arr[1]) == Catch::Approx(-1.0));
                REQUIRE(creal(arr[2]) == Catch::Approx(-0.5));

                REQUIRE(cimag(arr[0]) == Catch::Approx(sqrt(3.0)/2.0));
                REQUIRE(cimag(arr[1]) == Catch::Approx(0.0).margin(0.0001));
                REQUIRE(cimag(arr[2]) == Catch::Approx(-sqrt(3.0)/2.0));
            }
        }

        WHEN( "s-plane poles are computed with scaling 2.0" ) {
            compute_poles_on_s_plane(arr,3,2.0);
            THEN( "results are correct" ) {
                
                REQUIRE(creal(arr[0]) == Catch::Approx(-1.0));
                REQUIRE(creal(arr[1]) == Catch::Approx(-2.0));
                REQUIRE(creal(arr[2]) == Catch::Approx(-1.0));

                REQUIRE(cimag(arr[0]) == Catch::Approx(sqrt(3.0)));
                REQUIRE(cimag(arr[1]) == Catch::Approx(0.0).margin(0.0001));
                REQUIRE(cimag(arr[2]) == Catch::Approx(-sqrt(3.0)));
            }
        }
    }

}


SCENARIO( "Binomial coefficients can be computed", "[filter]" ) {
    GIVEN( "An array of size 0" ) {
        WHEN( "binomial coefficients are computed" ) {
            fill_array_with_binomial_coefficients(NULL,0);
            THEN( "nothing happens" ) {
            }
        }
    }

    GIVEN( "An array of size 1" ) {
        double arr[1] = {};

        WHEN( "binomial coefficients are computed" ) {
            fill_array_with_binomial_coefficients(arr,1);
            THEN( "the result is correct" ) {
                REQUIRE(arr[0] == 1.0);
            }
        }
    }

    GIVEN( "An array of size 2" ) {
        double arr[2] = {};

        WHEN( "binomial coefficients are computed" ) {
            fill_array_with_binomial_coefficients(arr,2);
            THEN( "the result is correct" ) {
                REQUIRE(arr[0] == 1.0);
                REQUIRE(arr[1] == 1.0);
            }
        }
    }

    GIVEN( "An array of size 3" ) {
        double arr[3] = {};

        WHEN( "binomial coefficients are computed" ) {
            fill_array_with_binomial_coefficients(arr,3);
            THEN( "the result is correct" ) {
                REQUIRE(arr[0] == 1.0);
                REQUIRE(arr[1] == 2.0);
                REQUIRE(arr[2] == 1.0);
            }
        }
    }

    GIVEN( "An array of size 5" ) {
        double arr[5] = {};

        WHEN( "binomial coefficients are computed" ) {
            fill_array_with_binomial_coefficients(arr,5);
            THEN( "the result is correct" ) {
                REQUIRE(arr[0] == 1.0);
                REQUIRE(arr[1] == 4.0);
                REQUIRE(arr[2] == 6.0);
                REQUIRE(arr[3] == 4.0);
                REQUIRE(arr[4] == 1.0);
            }
        }
    }

    GIVEN( "An array of size 6" ) {
        double arr[6] = {};

        WHEN( "binomial coefficients are computed" ) {
            fill_array_with_binomial_coefficients(arr,6);
            THEN( "the result is correct" ) {
                REQUIRE(arr[0] == 1.0);
                REQUIRE(arr[1] == 5.0);
                REQUIRE(arr[2] == 10.0);
                REQUIRE(arr[3] == 10.0);
                REQUIRE(arr[4] == 5.0);
                REQUIRE(arr[5] == 1.0);
            }
        }
    }

    GIVEN( "An array of size 10" ) {
        double arr[10] = {};

        WHEN( "binomial coefficients are computed" ) {
            fill_array_with_binomial_coefficients(arr,10);
            THEN( "the result is correct" ) {
                REQUIRE(arr[0] == 1.0);
                REQUIRE(arr[1] == 9.0);
                REQUIRE(arr[2] > arr[1]);
                REQUIRE(arr[3] > arr[2]);
                REQUIRE(arr[5] == arr[4]);
                REQUIRE(arr[6] == arr[3]);
                REQUIRE(arr[7] == arr[2]);
                REQUIRE(arr[8] == 9.0);
                REQUIRE(arr[9] == 1.0);
            }
        }
    }

    GIVEN( "An array of size 11" ) {
        double arr[11] = {};

        WHEN( "binomial coefficients are computed" ) {
            fill_array_with_binomial_coefficients(arr,11);
            THEN( "the result is correct" ) {
                REQUIRE(arr[0] == 1.0);
                REQUIRE(arr[1] == 10.0);
                REQUIRE(arr[2] > arr[1]);
                REQUIRE(arr[3] > arr[2]);
                REQUIRE(arr[4] > arr[3]);
                REQUIRE(arr[5] > arr[4]);
                REQUIRE(arr[6] == arr[4]);
                REQUIRE(arr[7] == arr[3]);
                REQUIRE(arr[8] == arr[2]);
                REQUIRE(arr[9] == 10.0);
                REQUIRE(arr[10] == 1.0);
            }
        }
    }
}

