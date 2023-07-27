cmake_minimum_required(VERSION 3.25)
project(
    String
    DESCRIPTION "Veilige string-interactions in C"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "-Werror")

set(SOURCES_DIR ./src)
set(TOP_LEVEL_SOURCES
    ${SOURCES_DIR}/main.c)

add_executable(String_Test
    ${TOP_LEVEL_SOURCES})

add_subdirectory(${SOURCES_DIR}/vector)
add_subdirectory(${SOURCES_DIR}/string)

target_link_libraries(String_Test
    PRIVATE
    string
    vector)