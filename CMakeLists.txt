cmake_minimum_required(VERSION 3.25)
include(FetchContent)
project(CString
    VERSION 0.8.1
    DESCRIPTION "Veilige string-interactions in C"
    LANGUAGES C)

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
