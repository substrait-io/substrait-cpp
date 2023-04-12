#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
SCRIPTDIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
WORKDIR="$( cd $SCRIPTDIR/.. && pwd )"

find  $WORKDIR -type d \( -path ./third_party \) -prune -o \( -name '*.cmake' -o -name 'CMakeLists.txt' \) | grep -v "third_party" | grep -v "/.cmake" | xargs cmake-format -i
cmake-format -i  $WORKDIR/third_party/CMakeLists.txt
