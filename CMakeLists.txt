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
cmake_minimum_required(VERSION 3.10)

# set the project name
project(substrait-cpp)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Protobuf 3.0.0 REQUIRED)

# GCC needs to link a library to enable std::filesystem.
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
  set(FILESYSTEM "stdc++fs")

  # Ensure we have gcc at least 8+.
  if(CMAKE_CXX_COMPILER_VERSION LESS 8.0)
    message(
      FATAL_ERROR "Substrait-CPP requires gcc > 8. Found ${CMAKE_CXX_COMPILER_VERSION}")
  endif()
else()
  set(FILESYSTEM "")
endif()

add_subdirectory(third_party)
add_subdirectory(substrait)
