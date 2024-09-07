# SPDX-License-Identifier: Apache-2.0

include_guard(GLOBAL)

set (BUILD_TZ_LIB ON CACHE BOOL "timezone library is a dependency" FORCE)
set (USE_SYSTEM_TZ_DB ON CACHE BOOL "use OS compiled timezone database" FORCE)
include(FetchContent)
FetchContent_Declare(date_src
        GIT_REPOSITORY https://github.com/HowardHinnant/date.git
        GIT_TAG v3.0.1
        )
FetchContent_MakeAvailable(date_src)
