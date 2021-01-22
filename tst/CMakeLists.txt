set(SRC test_dummy.cpp test_utils.cpp test_mindmap_builder.cpp test_parser.cpp)

################################################################################
# Make sure GTest is present and enable it
################################################################################
find_package(GTest REQUIRED)
include(GoogleTest)

################################################################################
# Definitions for the tests to find files
################################################################################
add_compile_definitions(TEST_ROOT="${CMAKE_BINARY_DIR}/tst/data/")
configure_file(data/test_mindmap.dot ${CMAKE_BINARY_DIR}/tst/data/test_mindmap.dot)

include_directories(${GTEST_INLUDE_DIRS})

# Build and executable. We can have one per test file or one for all test files
add_executable(dummy_t ${SRC})
target_link_libraries(dummy_t GTest::GTest GTest::Main mm mmparser)

# Let Google Test find the tests present in the executable
gtest_add_tests(TARGET dummy_t)
