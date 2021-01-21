################################################################################
# Utility to generate a png file from a Graphviz dot file
################################################################################
set(SRC dotpng.cpp)
add_executable(dotpng ${SRC})
target_link_libraries(dotpng mm)

set(MM_PARSE_LIBS mm mmparser)

################################################################################
# Utility to generate a dot file from a mindmap file
################################################################################
set(SRC mmdot.cpp)
add_executable(mmdot ${SRC})
target_link_libraries(mmdot ${MM_PARSE_LIBS})

################################################################################
# Utility to generate a png file from a mindmap file
################################################################################
set(SRC mmpng.cpp)
add_executable(mmpng ${SRC})
target_link_libraries(mmpng ${MM_PARSE_LIBS})

################################################################################
# Install all utilities. Binaries installed here get added to the deb package
################################################################################
install(TARGETS dotpng mmdot mmpng RUNTIME DESTINATION bin LIBRARY DESTINATION lib)


