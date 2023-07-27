add_library(vector "")

file(GLOB VECTOR_SOURCES *.c)
file(GLOB VECTOR_HEADERS *.h)

target_sources(vector
    PRIVATE
    ${VECTOR_SOURCES}
    PUBLIC
    ${VECTOR_HEADERS})

target_include_directories(vector
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR})

