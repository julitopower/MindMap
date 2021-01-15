

################################################################################
# Mindmap C library
################################################################################
set(SRC "cmm.c")
add_library(cmm STATIC ${SRC})

################################################################################
# Utilities C++ library
################################################################################
set(SRC "utils.cpp")
add_library(mm STATIC ${SRC})
target_link_libraries(mm gvc cgraph)

################################################################################
# Flex and Bison detection
################################################################################
find_package(FLEX)
find_package(BISON)



BISON_TARGET(parser grammar.y ${CMAKE_CURRENT_SOURCE_DIR}/parser.c)
FLEX_TARGET(scanner scanner.l ${CMAKE_CURRENT_SOURCE_DIR}/scanner.c)
ADD_FLEX_BISON_DEPENDENCY(scanner parser)

message(STATUS ${FLEX_scanner_OUTPUTS} ${BISON_parser_OUTPUTS})

add_executable(parser ${FLEX_scanner_OUTPUTS} ${BISON_parser_OUTPUTS})
target_link_libraries(parser cmm libfl.a)

add_library(mmparser ${FLEX_scanner_OUTPUTS} ${BISON_parser_OUTPUTS})
target_link_libraries(parser cmm libfl.a)
