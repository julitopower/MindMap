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
# BUILD_TESTS: Default ON. Build tests?
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

################################################################################
# CPack: Generate DEB package.
# See: https://cmake.org/cmake/help/latest/module/CPack.html
################################################################################
set(CPACK_GENERATOR "DEB")

# General information
set(CPACK_PACKAGE_NAME "mindmap")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Mindmap utilities")
set(CPACK_PACKAGE_CONTACT "Julio Delgado Mangas <julo.delgadomangas@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")

# Build and installation behavior
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_STRIP_FILES TRUE)
string(CONCAT DEB_PACKAGE_NAME
  ${CPACK_PACKAGE_NAME}-
  ${CPACK_PACKAGE_VERSION_MAJOR}-
  ${CPACK_PACKAGE_VERSION_MINOR}-
  ${CPACK_PACKAGE_VERSION_PATCH}.
  ${CMAKE_SYSTEM_NAME}-
  ${CMAKE_SYSTEM_PROCESSOR})  
set(CPACK_PACKAGE_FILE_NAME ${DEB_PACKAGE_NAME})
set(CPACK_DEBIAN_PACKAGE_DEPENDS graphviz)
include(CPack)
