# SPDX-License-Identifier: Apache-2.0

include_guard(GLOBAL)

set(ABSL_PROPAGATE_CXX_STD ON)

include(FetchContent)
FetchContent_Declare(GTest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
        OVERRIDE_FIND_PACKAGE
)
FetchContent_Declare(Protobuf
        GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
        GIT_TAG v28.2
        OVERRIDE_FIND_PACKAGE
)
set(protobuf_BUILD_TESTS OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(Protobuf GTest)
