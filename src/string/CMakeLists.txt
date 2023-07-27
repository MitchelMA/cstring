add_library(string "")

target_link_libraries(string
    PRIVATE
    vector)

file(GLOB STRING_SOURCES *.c)
file(GLOB STRING_HEADERS *.h)


target_sources(string
    PRIVATE
    ${STRING_SOURCES}
    PUBLIC
    ${STRING_HEADERS})

target_include_directories(string
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR})
