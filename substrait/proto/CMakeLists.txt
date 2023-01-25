#include(CMakePrintHelpers)



find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})



SET(PROTO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/third_party/substrait/proto/substrait")
cmake_path(GET PROTO_INCLUDE_DIR PARENT_PATH PROTO_INCLUDE_TOPLEVEL_DIR)
SET(PROTOBUF_IMPORT_DIRS ${PROTO_INCLUDE_DIR}/extensions)

file(GLOB PROTOBUF_FILELIST ${PROTO_INCLUDE_DIR}/*.proto ${PROTO_INCLUDE_DIR}/extensions/*.proto)
message(STATUS "PROTOBUF_FILELIST=${PROTOBUF_FILELIST}")


# Generate Protobuf cpp sources
set(PROTO_HDRS)
set(PROTO_SRCS)

file(RELATIVE_PATH CMAKE_RELATIVE_SOURCE_DIR ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
set(PROTO_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
#cmake_print_variables(PROTO_OUTPUT_DIR)
#cmake_print_variables(PROTO_INCLUDE_TOPLEVEL_DIR)

cmake_path(GET PROTO_OUTPUT_DIR PARENT_PATH PROTO_OUTPUT_PARENT_DIR)
cmake_path(GET PROTO_OUTPUT_PARENT_DIR PARENT_PATH PROTO_OUTPUT_TOPLEVEL_DIR)

#cmake_print_variables(CMAKE_CURRENT_BINARY_DIR)
#cmake_print_variables(PROTO_OUTPUT_TOPLEVEL_DIR)

foreach(PROTO_FILE IN LISTS PROTOBUF_FILELIST)
    #message(STATUS "protoc proto(cc): ${PROTO_FILE}")
    #get_filename_component(PROTO_DIR ${PROTO_FILE} DIRECTORY)
    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)

    # MEGAHACK -- Make sure extensions is in extensions subdirectory by using relative paths above instead of this hack.
   string(REPLACE "extensions" "extensions/extensions" PROTO_NAME ${PROTO_NAME})

    set(PROTO_HDR ${PROTO_OUTPUT_PARENT_DIR}/${PROTO_NAME}.pb.h)
    set(PROTO_SRC ${PROTO_OUTPUT_PARENT_DIR}/${PROTO_NAME}.pb.cc)
    #message(STATUS "protoc hdr: ${PROTO_HDR}")
    #message(STATUS "protoc src: ${PROTO_SRC}")
    add_custom_command(
            OUTPUT ${PROTO_SRC} ${PROTO_HDR}
            COMMAND protobuf::protoc
            "--proto_path=${PROTO_INCLUDE_TOPLEVEL_DIR}"
            "--cpp_out=${PROTO_OUTPUT_TOPLEVEL_DIR}"
            ${PROTO_FILE}
            DEPENDS ${PROTO_FILE} protobuf::protoc
            COMMENT "Generate C++ protocol buffer for ${PROTO_FILE}"
            VERBATIM)
    list(APPEND PROTO_HDRS ${PROTO_HDR})
    list(APPEND PROTO_SRCS ${PROTO_SRC})
endforeach()




add_library(proto ${PROTO_SRCS})

# Make sure we can see our own generated include files.
target_include_directories(proto PUBLIC "${PROTO_OUTPUT_TOPLEVEL_DIR}")

# MEGAHACK -- Probably not needed here as we don't perform a final link to make an executable.
target_link_libraries(proto ${Protobuf_LIBRARIES})
