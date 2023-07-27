cmake_minimum_required(VERSION 3.25)
project(
    String
    DESCRIPTION "Veilige string-interactions in C"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)

if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_C_FLAGS "-D_CRT_SECURE_NO_WARNINGS")
else()
    string(APPEND CMAKE_C_FLAGS "-pedantic -Wall -Wextra -Wconversion")
    if(${CMAKE_BUILD_TYPE_LOWER} MATCHES "release")
        string(APPEND CMAKE_C_FLAGS " -Werror")
    endif()
endif()

set(SOURCES_DIR ./src)
set(TOP_LEVEL_SOURCES
    ${SOURCES_DIR}/main.c)

add_executable(String_Example
    ${TOP_LEVEL_SOURCES})

add_subdirectory(${SOURCES_DIR}/vector)
add_subdirectory(${SOURCES_DIR}/string)

target_link_libraries(String_Example
    PRIVATE
    string
    vector)