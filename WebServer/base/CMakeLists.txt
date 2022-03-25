file(GLOB BASE_SRC *)

add_library(libserver_base ${BASE_SRC})
target_link_libraries(libserver_base pthread rt)

