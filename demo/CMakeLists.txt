###########################
# Compile demo library    #
###########################

add_ov_library(demo)
ov_library_includes(demo fb opcua)

##############################
# TODO Parse example nodeset #
##############################

# set(NODESET_DIRPREFIX ${CMAKE_SOURCE_DIR}/rte/syslibs/opcua/nodeset)
# set(open62541_TOOLS_DIR ${CMAKE_SOURCE_DIR}/rte/syslibs/opcua/open62541/tools)
# set(open62541_NODESET_DIR ${CMAKE_SOURCE_DIR}/rte/syslibs/opcua/open62541/deps/ua-nodeset)

# needed or cmake doesn't recognize dependencies of generated files
# set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR})

# ua_generate_nodeset_and_datatypes(
#     NAME "nodeset"
#     FILE_CSV "${NODESET_DIRPREFIX}/nodeset.csv"
#     FILE_BSD "${NODESET_DIRPREFIX}/nodeset.bsd"
#     NAMESPACE_MAP "2:http://github.com/acplt/rte_opcua_example/Nodeset/"
#     FILE_NS "${NODESET_DIRPREFIX}/nodeset.xml"
#     INTERNAL
# )
# target_sources(demo PRIVATE ${UA_NODESET_NODESET_SOURCES} ${UA_TYPES_NODESET_SOURCES})
# add_dependencies(demo open62541-generator-ns-nodeset)