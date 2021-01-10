set(SRC dotpng.cpp)

add_executable(dotpng ${SRC})
target_link_libraries(dotpng mm)

install(TARGETS dotpng RUNTIME DESTINATION bin LIBRARY DESTINATION lib)


