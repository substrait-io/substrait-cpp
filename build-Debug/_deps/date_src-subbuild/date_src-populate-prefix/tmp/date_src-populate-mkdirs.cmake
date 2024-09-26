# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(
  MAKE_DIRECTORY
  "/home/davids/projects/substrait-cpp/build-Debug/_deps/date_src-src"
  "/home/davids/projects/substrait-cpp/build-Debug/_deps/date_src-build"
  "/home/davids/projects/substrait-cpp/build-Debug/_deps/date_src-subbuild/date_src-populate-prefix"
  "/home/davids/projects/substrait-cpp/build-Debug/_deps/date_src-subbuild/date_src-populate-prefix/tmp"
  "/home/davids/projects/substrait-cpp/build-Debug/_deps/date_src-subbuild/date_src-populate-prefix/src/date_src-populate-stamp"
  "/home/davids/projects/substrait-cpp/build-Debug/_deps/date_src-subbuild/date_src-populate-prefix/src"
  "/home/davids/projects/substrait-cpp/build-Debug/_deps/date_src-subbuild/date_src-populate-prefix/src/date_src-populate-stamp"
)

set(configSubDirs)
foreach(subDir IN LISTS configSubDirs)
  file(
    MAKE_DIRECTORY
    "/home/davids/projects/substrait-cpp/build-Debug/_deps/date_src-subbuild/date_src-populate-prefix/src/date_src-populate-stamp/${subDir}"
  )
endforeach()
if(cfgdir)
  file(
    MAKE_DIRECTORY
    "/home/davids/projects/substrait-cpp/build-Debug/_deps/date_src-subbuild/date_src-populate-prefix/src/date_src-populate-stamp${cfgdir}"
  ) # cfgdir has leading slash
endif()
