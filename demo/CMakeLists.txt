
add_ov_library(demo)
ov_library_includes(demo fb opcua)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(demo PRIVATE -lm)
endif()
