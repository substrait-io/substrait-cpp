# SPDX-License-Identifier: Apache-2.0

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(Perl REQUIRED)
SET(UPDATE_PROTO_PACKAGE_TOOL "${CMAKE_CURRENT_SOURCE_DIR}/update_proto_package.pl")

SET(PROTO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/third_party/substrait/proto/substrait")
cmake_path(GET PROTO_INCLUDE_DIR PARENT_PATH PROTO_INCLUDE_TOPLEVEL_DIR)
SET(PROTOBUF_IMPORT_DIRS ${PROTO_INCLUDE_DIR}/extensions)

file(GLOB PROTOBUF_FILELIST ${PROTO_INCLUDE_DIR}/*.proto ${PROTO_INCLUDE_DIR}/extensions/*.proto)


# Create a copy of all of the input proto files with an updated package (substrait.proto).
SET(GENERATED_PROTO_TOPLEVEL_DIR "${CMAKE_BINARY_DIR}/substrait/proto/generated-protos")
SET(GENERATED_PROTO_MIDLEVEL_DIR "${GENERATED_PROTO_TOPLEVEL_DIR}/substrait/proto")
SET(GENERATED_PROTOBUF_LIST)
foreach(PROTO_FILE IN LISTS PROTOBUF_FILELIST)
    file(RELATIVE_PATH RELATIVE_PROTO_PATH "${CMAKE_SOURCE_DIR}/third_party/substrait/proto/substrait" "${PROTO_FILE}")
    set(GENERATED_PROTO_FILE ${GENERATED_PROTO_MIDLEVEL_DIR}/${RELATIVE_PROTO_PATH})

    cmake_path(GET GENERATED_PROTO_FILE PARENT_PATH GENERATED_PROTO_DIR)
    add_custom_command(
            OUTPUT ${GENERATED_PROTO_FILE}
            COMMAND mkdir -p ${GENERATED_PROTO_DIR}
            COMMAND ${PERL_EXECUTABLE} ${UPDATE_PROTO_PACKAGE_TOOL} ${PROTO_FILE} > ${GENERATED_PROTO_FILE}
            DEPENDS ${PROTO_FILE} ${UPDATE_PROTO_PACKAGE_TOOL}
            COMMENT "Modified package name in protobuf definition for ${PROTO_FILE}."
            VERBATIM)
    list(APPEND GENERATED_PROTOBUF_LIST ${GENERATED_PROTO_FILE})
endforeach()


# Generate cpp sources for the protobufs in third_party/substrait/proto/substrait.
set(PROTO_HDRS)
set(PROTO_SRCS)

set(PROTO_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")

cmake_path(GET PROTO_OUTPUT_DIR PARENT_PATH PROTO_OUTPUT_PARENT_DIR)
cmake_path(GET PROTO_OUTPUT_PARENT_DIR PARENT_PATH PROTO_OUTPUT_TOPLEVEL_DIR)

foreach(PROTO_FILE IN LISTS PROTOBUF_FILELIST)
    file(RELATIVE_PATH RELATIVE_PROTO_PATH "${CMAKE_SOURCE_DIR}/third_party/substrait/proto/substrait" "${PROTO_FILE}")
    set(GENERATED_PROTO_FILE ${GENERATED_PROTO_MIDLEVEL_DIR}/${RELATIVE_PROTO_PATH})
    message(STATUS "Defined C++ source/header build rule for ${RELATIVE_PROTO_PATH}")

    cmake_path(REMOVE_EXTENSION RELATIVE_PROTO_PATH)

    set(PROTO_HDR ${PROTO_OUTPUT_PARENT_DIR}/proto/${RELATIVE_PROTO_PATH}.pb.h)
    set(PROTO_SRC ${PROTO_OUTPUT_PARENT_DIR}/proto/${RELATIVE_PROTO_PATH}.pb.cc)
    add_custom_command(
            OUTPUT ${PROTO_SRC} ${PROTO_HDR}
            COMMAND protobuf::protoc
            "--proto_path=${GENERATED_PROTO_TOPLEVEL_DIR}"
            "--cpp_out=${PROTO_OUTPUT_TOPLEVEL_DIR}"
            ${GENERATED_PROTO_FILE}
            DEPENDS ${GENERATED_PROTOBUF_LIST} protobuf::protoc
            COMMENT "Generated C++ protobuf module for ${PROTO_FILE}"
            VERBATIM)
    list(APPEND PROTO_HDRS ${PROTO_HDR})
    list(APPEND PROTO_SRCS ${PROTO_SRC})
endforeach()

# Add the generated protobuf C++ files to our exported library.
add_library(substrait_proto ${PROTO_SRCS})

# Make sure we can see our own generated include files.
target_include_directories(substrait_proto PUBLIC "${PROTO_OUTPUT_TOPLEVEL_DIR}")
