cmake_minimum_required(VERSION 3.25)
include(FetchContent)
project(CString
    VERSION 0.8.1
    DESCRIPTION "Veilige string-interactions in C"
    LANGUAGES C)

set(CMAKE_C_STANDARD 17)

if (TARGET CVector)
else()
    FetchContent_Declare(CVector
        GIT_REPOSITORY https://github.com/MitchelMA/CVector.git
        GIT_TAG main)
    FetchContent_MakeAvailable(CVector)
endif()

set(SOURCES_DIR ./libsrc)

add_subdirectory(${SOURCES_DIR})

target_compile_options(CString
    PUBLIC
    -std=gnu17)
