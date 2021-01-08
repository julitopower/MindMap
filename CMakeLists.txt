################################################################################
# Since we use gtest_add_tests, we require at least version 3.9
#
# https://cmake.org/cmake/help/latest/module/GoogleTest.html
################################################################################
cmake_minimum_required(VERSION 3.9)
project(MindMap)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

################################################################################
# All the options go here
#
# BUILD_TEST: Default ON. Build tests?
#
################################################################################
option(BUILD_TESTS "Do we also buitd the tests?" ON)

include_directories(include)

add_subdirectory(bin)
add_subdirectory(src)

################################################################################
# Only build tests if the corresponding flag is on
################################################################################
if (BUILD_TESTS)
  message(STATUS "## MSG ## Build includes tests")
  enable_testing()  
  add_subdirectory(tst)
endif()


