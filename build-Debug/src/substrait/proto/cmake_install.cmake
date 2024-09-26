# Install script for directory: /home/davids/projects/substrait-cpp/src/substrait/proto

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" "" CMAKE_INSTALL_CONFIG_NAME
                         "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT
                                                     CMAKE_INSTALL_COMPONENT)
  file(
    INSTALL
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    TYPE
    STATIC_LIBRARY
    FILES
    "/home/davids/projects/substrait-cpp/build-Debug/debug/lib/libsubstrait_proto.a"
  )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT
                                                     CMAKE_INSTALL_COMPONENT)
  file(
    INSTALL
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include/substrait/proto"
    TYPE
    FILE
    FILES
    "/home/davids/projects/substrait-cpp/build-Debug/src/substrait/proto/algebra.pb.h"
    "/home/davids/projects/substrait-cpp/build-Debug/src/substrait/proto/capabilities.pb.h"
    "/home/davids/projects/substrait-cpp/build-Debug/src/substrait/proto/extended_expression.pb.h"
    "/home/davids/projects/substrait-cpp/build-Debug/src/substrait/proto/function.pb.h"
    "/home/davids/projects/substrait-cpp/build-Debug/src/substrait/proto/parameterized_types.pb.h"
    "/home/davids/projects/substrait-cpp/build-Debug/src/substrait/proto/plan.pb.h"
    "/home/davids/projects/substrait-cpp/build-Debug/src/substrait/proto/type.pb.h"
    "/home/davids/projects/substrait-cpp/build-Debug/src/substrait/proto/type_expressions.pb.h"
    "/home/davids/projects/substrait-cpp/src/substrait/proto/ProtoUtils.h")
  file(
    INSTALL
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include/substrait/proto/extensions"
    TYPE
    FILE
    FILES
    "/home/davids/projects/substrait-cpp/build-Debug/src/substrait/proto/extensions/extensions.pb.h"
  )
endif()
