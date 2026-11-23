#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
SCRIPTDIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
WORKDIR="$( cd $SCRIPTDIR/.. && pwd )"

CLANG_FORMAT="clang-format-15"
if ! command -v "$CLANG_FORMAT" &> /dev/null; then
  echo "Error: clang-format-15 not found. Please install clang-format-15 to match CI formatting." >&2
  exit 1
fi

find $WORKDIR/src $WORKDIR/include \( -name '*.h' -o -name '*.cpp' \) -exec "$CLANG_FORMAT" -style=file -i {} \;
