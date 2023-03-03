#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
SCRIPTDIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
WORKDIR="$( cd $SCRIPTDIR/.. && pwd )"

# Make compile_command.json
rm -rf tmp && mkdir tmp && cmake -Btmp -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
# Build substrait protocols
pushd tmp/src/substrait/proto && make -j && popd || exit
# Run clang-tidy
if [ "$1" == "fix" ]; then
   python3 scripts/run-clang-tidy.py "$WORKDIR" "tmp" "third_party" "h,hpp,cc,cpp" "--quiet --fix --fix-errors"
else
   python3 scripts/run-clang-tidy.py "$WORKDIR" "tmp" "third_party" "h,hpp,cc,cpp" "--quiet"
fi