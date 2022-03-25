file(GLOB SRC *)

include_directories(include_directories(${PROJECT_SOURCE_DIR}/base))

add_executable(WebServer ${SRC})
add_subdirectory(base)