
cmake_minimum_required(VERSION 3.14)

# This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.

project(FilterGen VERSION 1.0.0)

option(DH_CFILTER_BUILD_CPP_BINDINGS "If the bindings for cpp should be built." ON)
option(DH_CFILTER_BUILD_JS_BINDINGS "If the bindings for javascript should be built. This includes the C++ Bindings." OFF)
option(DH_CFILTER_BUILD_TESTS "If the tests should be built." ON)
option(DH_CFILTER_BUILD_EXAMPLES "If the example application should be built." ON)

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

find_package(Doxygen)
if(DH_CFILTER_BUILD_TESTS OR DH_CFILTER_BUILD_EXAMPLES OR TARGET Doxygen::doxygen)
  Include(FetchContent)
endif()

if(MSVC)
  target_compile_options(filter PRIVATE /W4 /WX)
else()
  target_compile_options(filter PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if(DH_CFILTER_BUILD_CPP_BINDINGS OR DH_CFILTER_BUILD_JS_BINDINGS)
  add_library(filter_cpp
    src/bindings/cpp/filter.cpp
  )
  target_link_libraries(filter_cpp PUBLIC dh::filter)
  add_library(dh::filter_cpp ALIAS filter_cpp)
  if(MSVC)
    target_compile_options(filter_cpp PRIVATE /W4 /WX)
  else()
    target_compile_options(filter_cpp PRIVATE -Wall -Wextra -Wpedantic -Werror)
  endif()
endif()

if(DH_CFILTER_BUILD_EXAMPLES AND DH_CFILTER_BUILD_CPP_BINDINGS)
  FetchContent_Declare(
    Cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG        v3.0.0
  )
  FetchContent_MakeAvailable(Cxxopts)

  add_executable(design-filter
    examples/design-filter.cpp
  )
  target_link_libraries(design-filter PRIVATE cxxopts::cxxopts dh::filter_cpp)

endif()

if(DH_CFILTER_BUILD_JS_BINDINGS)
  add_executable(filter.js
    src/bindings/js/emscripten.cpp
  )
  target_link_libraries(filter.js PRIVATE -lembind -Wl,--whole-archive dh::filter dh::filter_cpp -Wl,--no-whole-archive)
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
    test/chebyshev2-test.cpp
    test/moving-average-test.cpp
    test/generated_c_code.c
    test/generated-code-test.cpp
    test/iir-filter-test.cpp
    test/fir-exponential-test.cpp
    test/iir-exponential-test.cpp
    test/utility-test.cpp
  )
  target_link_libraries(test-filter PRIVATE Catch2::Catch2WithMain dh::filter)
  if(DH_CFILTER_BUILD_CPP_BINDINGS)
    target_sources(test-filter PRIVATE test/cpp-bindings-test.cpp)
    target_link_libraries(test-filter PRIVATE dh::filter_cpp)
  endif()
endif()

if(TARGET Doxygen::doxygen)
  FetchContent_Declare(
    doxygen-awesome
    GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
    GIT_TAG        v2.0.3
  )
  FetchContent_MakeAvailable(doxygen-awesome)

  set(DOXYGEN_STRIP_FROM_INC_PATH ${PROJECT_SOURCE_DIR}/include)
  set(DOXYGEN_STRIP_FROM_PATH ${PROJECT_SOURCE_DIR}/include)
  set(DOXYGEN_HTML_EXTRA_STYLESHEET "${CMAKE_CURRENT_BINARY_DIR}/_deps/doxygen-awesome-src/doxygen-awesome.css;${CMAKE_CURRENT_BINARY_DIR}/_deps/doxygen-awesome-src/doxygen-awesome-sidebar-only.css")
  set(DOXYGEN_GENERATE_TREEVIEW "YES")
  set(DOXYGEN_PROJECT_BRIEF "A library to design digital filters in embedded systems.")
  set(DOXYGEN_PROJECT_LOGO "docs/assets/images/favicon-32x32.png")
  set(DOXYGEN_OUTPUT_DIRECTORY "docs/api")
  set(DOXYGEN_QUIET "YES")

  doxygen_add_docs(apidocs
    ${PROJECT_SOURCE_DIR}/include/
    COMMENT "Generating API documentation"
  )
endif()
