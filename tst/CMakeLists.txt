set(SRC test_dummy.cpp)

################################################################################
# Make sure GTest is present and enable it
################################################################################
find_package(GTest REQUIRED)
include(GoogleTest)

# Build and executable. We can have one per test file or one for all test files
add_executable(dummy_t ${SRC})
target_link_libraries(dummy_t GTest::GTest GTest::Main)

# Let Google Test find the tests present in the executable
gtest_add_tests(TARGET dummy_t)
