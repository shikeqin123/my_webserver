file(GLOB BASE_SRC *)

add_library(libserver_base ${BASE_SRC})
target_link_libraries(libserver_base pthread rt)

set_target_properties(libserver_base PROPERTIES OUTPUT_NAME "server_base")

add_subdirectory(test)