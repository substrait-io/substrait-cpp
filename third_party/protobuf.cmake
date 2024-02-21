# SPDX-License-Identifier: Apache-2.0

include_guard(GLOBAL)

set(ABSL_PROPAGATE_CXX_STD ON)

include(FetchContent)
FetchContent_Declare(Protobuf
        GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
        GIT_TAG v4.25.3
        OVERRIDE_FIND_PACKAGE
        )
FetchContent_MakeAvailable(Protobuf)
