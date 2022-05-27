#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"
#include "generated_c_code.h"
#include <cstring>

/**
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

SCENARIO( "An buttwerworth lowpass filter generated on the webpage can be used", "[filter]" ) {
    GIVEN("Generated code"){
        filter_data data;
        std::memset(&data, 0, sizeof(filter_data));
        WHEN("A zero is filtered"){
            double output = update_filter(&data,0.0);
            THEN("Output is 0"){
                REQUIRE(output == 0.0);
            }
        }

        WHEN("A step function is filtered"){
            double output = update_filter(&data,100.0);
            THEN("Output is larger than 0"){
                REQUIRE(output > 0.0);
                REQUIRE(output < 100.0);
            }
            double output2 = update_filter(&data,100.0);
            THEN("Output is larger than before"){
                REQUIRE(output2 > output);
                REQUIRE(output2 < 100.0);
            }
        }
    }
}
