# Copyright (c) Facebook, Inc. and its affiliates.
#
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

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${PROTO_INCLUDE_DIR} ${substrait_textplan_INCLUDE_DIRS})
add_executable(
        textplan_conversion_test
        BinaryToTextPlanConversionTest.cpp)


add_dependencies(textplan_conversion_test substrait_textplan)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH TEXTPLAN_SOURCE_DIR)

add_test(
        NAME textplan_conversion_test
        COMMAND textplan_conversion_test
        WORKING_DIRECTORY ${TEXTPLAN_SOURCE_DIR})

target_link_libraries(
        textplan_conversion_test
        substrait_common
        substrait_textplan
        ${PROTOBUF_LIBRARIES}
        proto
        gtest
        gtest_main)
