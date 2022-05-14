
cmake_minimum_required(VERSION 3.14)

# This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.

project(CFilter VERSION 1.0.0)

option(DH_CFILTER_BUILD_TESTS "If the tests should be built." OFF)
option(DH_CFILTER_BUILD_EXAMPLES "If the example application should be built." OFF)

add_library(filter 
  src/filter.c
  src/utility.c
  src/create_filter.c
  src/free_filter.c
  src/butterworth.c
  src/chebyshev.c
)
target_include_directories(filter PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

add_library(dh::filter ALIAS filter)

if(DH_CFILTER_BUILD_TESTS OR DH_CFILTER_BUILD_EXAMPLES)
  Include(FetchContent)
endif()

if(DH_CFILTER_BUILD_EXAMPLES)
  FetchContent_Declare(
    Cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG        v3.0.0
  )
  FetchContent_MakeAvailable(Cxxopts)

  add_executable(design-filter
    examples/design-filter.cpp
  )
  target_link_libraries(design-filter PRIVATE cxxopts::cxxopts dh::filter)

endif()


if(DH_CFILTER_BUILD_TESTS)
  Include(FetchContent)

  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.0.0-preview4
  )

  FetchContent_MakeAvailable(Catch2)

  add_executable(test-filter
    test/brickwall-test.cpp
    test/butterworth-test.cpp
    test/chebyshev-test.cpp
    test/moving-average-test.cpp
    test/iir-filter-test.cpp
    test/fir-exponential-test.cpp
    test/iir-exponential-test.cpp
    test/utility-test.cpp
  )
  target_link_libraries(test-filter PRIVATE Catch2::Catch2WithMain dh::filter)
endif()
