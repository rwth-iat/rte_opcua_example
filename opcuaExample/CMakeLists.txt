
add_ov_library(opcuaExample)
ov_library_includes(opcuaExample fb opcua)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(opcuaExample PRIVATE -lm)
endif()
