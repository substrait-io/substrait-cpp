#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
SCRIPTDIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
WORKDIR="$( cd $SCRIPTDIR/.. && pwd )"

(
    cd "$WORKDIR" && \
    git ls-files "*.cmake" "CMakeLists.txt" "**/*.cmake" "**/CMakeLists.txt" \
    | grep -v third_party/antlr4 \
    | xargs cmake-format -i
)