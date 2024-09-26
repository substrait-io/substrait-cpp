# Install script for directory: /home/davids/projects/substrait-cpp/src/substrait/textplan/converter

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include(
    "/home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/converter/tests/cmake_install.cmake"
  )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT
                                                     CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/planconverter"
     AND NOT IS_SYMLINK
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/planconverter")
    file(RPATH_CHECK FILE
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/planconverter" RPATH "")
  endif()
  file(
    INSTALL
    DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    TYPE EXECUTABLE FILES
    "/home/davids/projects/substrait-cpp/build-Debug/debug/planconverter")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/planconverter"
     AND NOT IS_SYMLINK
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/planconverter")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(
        COMMAND "/usr/bin/strip"
                "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/planconverter")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT
                                                     CMAKE_INSTALL_COMPONENT)
  file(
    INSTALL
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    TYPE
    STATIC_LIBRARY
    FILES
    "/home/davids/projects/substrait-cpp/build-Debug/debug/lib/libsubstrait_textplan_converter.a"
  )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT
                                                     CMAKE_INSTALL_COMPONENT)
  file(
    INSTALL
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    TYPE
    STATIC_LIBRARY
    FILES
    "/home/davids/projects/substrait-cpp/build-Debug/debug/lib/libsubstrait_base_proto_visitor.a"
  )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT
                                                     CMAKE_INSTALL_COMPONENT)
  file(
    INSTALL
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    TYPE
    STATIC_LIBRARY
    FILES
    "/home/davids/projects/substrait-cpp/build-Debug/debug/lib/libsubstrait_textplan_normalizer.a"
  )
endif()
