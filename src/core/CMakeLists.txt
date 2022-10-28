# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


# Set up Proto
set(PROTO_DIRECTORY ${CMAKE_SOURCE_DIR}/third_party/substrait/proto)
set(SUBSTRAIT_PROTO_DIRECTORY ${PROTO_DIRECTORY}/substrait)
set(PROTO_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/third_party/substrait/proto")
file(GLOB PROTO_FILES ${SUBSTRAIT_PROTO_DIRECTORY}/*.proto
        ${SUBSTRAIT_PROTO_DIRECTORY}/extensions/*.proto)
foreach (PROTO ${PROTO_FILES})
    file(RELATIVE_PATH REL_PROTO ${SUBSTRAIT_PROTO_DIRECTORY} ${PROTO})
    string(REGEX REPLACE "\\.proto" "" PROTO_NAME ${REL_PROTO})
    list(APPEND PROTO_SRCS "${PROTO_OUTPUT_DIR}/substrait/${PROTO_NAME}.pb.cc")
    list(APPEND PROTO_HDRS "${PROTO_OUTPUT_DIR}/substrait/${PROTO_NAME}.pb.h")
endforeach ()
set(PROTO_OUTPUT_FILES ${PROTO_HDRS} ${PROTO_SRCS})
set_source_files_properties(${PROTO_OUTPUT_FILES} PROPERTIES GENERATED TRUE)
get_filename_component(PROTO_DIR ${SUBSTRAIT_PROTO_DIRECTORY}/, DIRECTORY)

# Generate Substrait hearders
add_custom_command(
        OUTPUT ${PROTO_OUTPUT_FILES}
        COMMAND protoc --proto_path ${PROTO_DIRECTORY}/ --cpp_out ${PROTO_OUTPUT_DIR}
        ${PROTO_FILES}
        DEPENDS ${PROTO_DIR}
        COMMENT "Running PROTO compiler"
        VERBATIM)

include_directories(${PROTO_OUTPUT_DIR})

set(SRCS
        Type.cpp
        Function.cpp
        Extension.cpp
        FunctionMapping.h
        FunctionLookup.cpp
        FunctionSignature.h)

add_library(substrait-core ${SRCS})

target_link_libraries(
        substrait-core
        substrait_common
        yaml-cpp)

if (${BUILD_TESTING})
    add_subdirectory(tests)
endif ()
