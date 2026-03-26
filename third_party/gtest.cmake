# SPDX-License-Identifier: Apache-2.0

include_guard(GLOBAL)

include(FetchContent)
fetchcontent_declare(
  GTest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
  OVERRIDE_FIND_PACKAGE)

# Disable warnings for dependency targets.
if(MSVC)
  set(gtest_force_shared_crt ON)
  add_compile_options("/W0")
else()
  add_compile_options("-w")
endif()
fetchcontent_makeavailable(GTest)
