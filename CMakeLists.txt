cmake_minimum_required(VERSION 3.25)
include(FetchContent)
project(CString
    VERSION 0.8.1
    DESCRIPTION "Veilige string-interactions in C"
    LANGUAGES C)

if (PROJECT_IS_TOP_LEVEL)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# Checking if the CVector target already exists
if (TARGET CVector)
else()
    FetchContent_Declare(CVector
        GIT_REPOSITORY https://github.com/MitchelMA/CVector.git
        GIT_TAG main)
    FetchContent_MakeAvailable(CVector)
endif()

add_subdirectory(./libsrc)

target_compile_options(CString
    PRIVATE 
    -std=gnu17)

if (PROJECT_IS_TOP_LEVEL)
    target_compile_options(CString
        PRIVATE
        -Wall -Wextra -Wpedantic -pedantic -Wundef
        $<$<STREQUAL:$<UPPER_CASE:${CMAKE_BUILD_TYPE}>,DEBUG>:-g -ggdb>
        $<$<STREQUAL:$<UPPER_CASE:${CMAKE_BUILD_TYPE}>,RELEASE>:-Werror -O3 -ffast-math>)
endif()
