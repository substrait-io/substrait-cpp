#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
SCRIPTDIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
WORKDIR="$( cd $SCRIPTDIR/.. && pwd )"

CLANG_FORMAT="clang-format-15"
if ! command -v "$CLANG_FORMAT" &> /dev/null; then
  CLANG_FORMAT="clang-format"
fi

if ! command -v "$CLANG_FORMAT" &> /dev/null; then
  echo "Error: clang-format not found. Please install clang-format (e.g. clang-format-15 or clang-tools-extra)." >&2
  exit 1
fi

find $WORKDIR/src $WORKDIR/include \( -name '*.h' -o -name '*.cpp' \) -exec "$CLANG_FORMAT" -style=file -i {} \;
