set(SRC "utils.cpp")

add_library(mm STATIC ${SRC})
target_link_libraries(mm gvc cgraph)
