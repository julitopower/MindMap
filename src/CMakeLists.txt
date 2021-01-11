set(SRC "utils.cpp")

################################################################################
# Flex and Bison detection
################################################################################
find_package(FLEX)
find_package(BISON)

add_library(mm STATIC ${SRC})
target_link_libraries(mm gvc cgraph)

FLEX_TARGET(scanner scanner.l ${CMAKE_CURRENT_SOURCE_DIR}/scanner.cpp)
add_executable(scanner ${FLEX_scanner_OUTPUTS})
target_link_libraries(scanner libfl.a)
