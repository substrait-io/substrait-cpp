#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
SCRIPTDIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
WORKDIR="$( cd $SCRIPTDIR/.. && pwd )"

find $WORKDIR/src $WORKDIR/include \( -name '*.h' -o -name '*.cpp' \) -exec clang-format -style=file -i {} \;