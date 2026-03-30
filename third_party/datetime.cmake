# SPDX-License-Identifier: Apache-2.0

include_guard(GLOBAL)

set(BUILD_TZ_LIB
    ON
    CACHE BOOL "timezone library is a dependency" FORCE)
set(USE_SYSTEM_TZ_DB
    ON
    CACHE BOOL "use OS compiled timezone database" FORCE)
include(FetchContent)
fetchcontent_declare(
  date_src
  GIT_REPOSITORY https://github.com/HowardHinnant/date.git
  GIT_TAG v3.0.1
  CMAKE_ARGS -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
  -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER})
fetchcontent_makeavailable(date_src)
