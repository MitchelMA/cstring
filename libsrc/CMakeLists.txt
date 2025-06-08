add_library(CString)

target_link_libraries(CString
    PUBLIC 
    CVector)

file(GLOB STRING_SOURCES ./private/*.c)


target_sources(CString
    PRIVATE
    ${STRING_SOURCES})

target_include_directories(CString
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include)
