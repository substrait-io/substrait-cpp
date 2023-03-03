#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
SCRIPTDIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
WORKDIR="$( cd $SCRIPTDIR/.. && pwd )"

find $WORKDIR \( -name '*.cmake' -o -name 'CMakeLists.txt' \) -type f  -not -path './third_party/*/**'| xargs cmake-format -i
