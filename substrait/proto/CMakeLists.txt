# SPDX-License-Identifier: Apache-2.0

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(Perl REQUIRED)

SET(PROTO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/third_party/substrait/proto/substrait")
cmake_path(GET PROTO_INCLUDE_DIR PARENT_PATH PROTO_INCLUDE_TOPLEVEL_DIR)
SET(PROTOBUF_IMPORT_DIRS ${PROTO_INCLUDE_DIR}/extensions)

file(GLOB PROTOBUF_FILELIST ${PROTO_INCLUDE_DIR}/*.proto ${PROTO_INCLUDE_DIR}/extensions/*.proto)


# Generate cpp sources for the protobufs in third_party/substrait/proto/substrait.
set(PROTO_HDRS)
set(PROTO_SRCS)

set(PROTO_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")

cmake_path(GET PROTO_OUTPUT_DIR PARENT_PATH PROTO_OUTPUT_PARENT_DIR)
cmake_path(GET PROTO_OUTPUT_PARENT_DIR PARENT_PATH PROTO_OUTPUT_TOPLEVEL_DIR)

foreach(PROTO_FILE IN LISTS PROTOBUF_FILELIST)
    file(RELATIVE_PATH RELATIVE_PROTO_PATH "${CMAKE_SOURCE_DIR}/third_party/substrait/proto/substrait" "${PROTO_FILE}")
    message(STATUS "Creating C++ source/header build rule for ${RELATIVE_PROTO_PATH}")

    cmake_path(REMOVE_EXTENSION RELATIVE_PROTO_PATH)

    set(PROTO_GENERATED_HDR ${PROTO_OUTPUT_PARENT_DIR}/${RELATIVE_PROTO_PATH}.pb.h)
    set(PROTO_GENERATED_SRC ${PROTO_OUTPUT_PARENT_DIR}/${RELATIVE_PROTO_PATH}.pb.cc)
    set(PROTO_HDR ${PROTO_OUTPUT_PARENT_DIR}/proto/${RELATIVE_PROTO_PATH}.pb.h)
    set(PROTO_SRC ${PROTO_OUTPUT_PARENT_DIR}/proto/${RELATIVE_PROTO_PATH}.pb.cc)
    add_custom_command(
            OUTPUT ${PROTO_SRC} ${PROTO_HDR}
            COMMAND protobuf::protoc
            "--proto_path=${PROTO_INCLUDE_TOPLEVEL_DIR}"
            "--cpp_out=${PROTO_OUTPUT_TOPLEVEL_DIR}"
            ${PROTO_FILE}
            COMMAND mkdir -p "${PROTO_OUTPUT_PARENT_DIR}/proto"
            COMMAND mv ${PROTO_GENERATED_HDR} ${PROTO_HDR}
            COMMAND ${PERL_EXECUTABLE} -i "${CMAKE_CURRENT_SOURCE_DIR}/update_proto_includes.py" ${PROTO_HDR}
            COMMAND mv ${PROTO_GENERATED_SRC} ${PROTO_SRC}
            COMMAND ${PERL_EXECUTABLE} -i "${CMAKE_CURRENT_SOURCE_DIR}/update_proto_includes.py" ${PROTO_SRC}
            DEPENDS ${PROTO_FILE} protobuf::protoc "${CMAKE_CURRENT_SOURCE_DIR}/update_proto_includes.py"
            COMMENT "Generate C++ protocol buffer for ${PROTO_FILE}"
            VERBATIM)
    list(APPEND PROTO_HDRS ${PROTO_HDR})
    list(APPEND PROTO_SRCS ${PROTO_SRC})
endforeach()

# Add the generated protobuf C++ files to our exported library.
add_library(substrait_proto ${PROTO_SRCS})

# Make sure we can see our own generated include files.
target_include_directories(substrait_proto PUBLIC "${PROTO_OUTPUT_TOPLEVEL_DIR}")
