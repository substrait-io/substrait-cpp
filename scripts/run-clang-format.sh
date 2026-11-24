#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
SCRIPTDIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
WORKDIR="$( cd $SCRIPTDIR/.. && pwd )"

# CI formats with clang-format-15. Set CLANG_FORMAT to point at a clang-format 15
# binary (e.g. on distributions that do not package it under this name).
CLANG_FORMAT="${CLANG_FORMAT:-clang-format-15}"
if ! command -v "$CLANG_FORMAT" &> /dev/null; then
  echo "Error: $CLANG_FORMAT not found. Install clang-format 15 (or set CLANG_FORMAT to its path) to match CI formatting." >&2
  exit 1
fi

find $WORKDIR/src $WORKDIR/include \( -name '*.h' -o -name '*.cpp' \) -exec "$CLANG_FORMAT" -style=file -i {} \;
