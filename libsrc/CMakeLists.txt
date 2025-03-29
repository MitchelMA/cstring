add_library(CString)

target_link_libraries(CString
    PUBLIC 
    CVector)

file(GLOB STRING_SOURCES ./private/*.c)
file(GLOB STRING_HEADERS ./include/*.h)


target_sources(CString
    PRIVATE
    ${STRING_SOURCES}
    PUBLIC
    ${STRING_HEADERS})

target_include_directories(CString
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include)
