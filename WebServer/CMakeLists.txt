file(GLOB SRC *)

include_directories(${PROJECT_SOURCE_DIR}/base)

add_executable(WebServer ${SRC})
target_link_libraries(WebServer libserver_base)

add_subdirectory(base)