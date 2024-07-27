#!/bin/bash
# SPDX-License-Identifier: Apache-2.0
SCRIPTDIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
WORKDIR="$( cd $SCRIPTDIR/.. && pwd )"

set -o errexit

echo Making compile_command.json
rm -rf tmp && mkdir tmp && cmake -Btmp -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DSUBSTRAIT_CPP_BUILD_TESTING=OFF -DCLANG_TIDY_BUILD=ON
echo Building substrait protobuf
pushd tmp/src/substrait/proto
make -j
popd
echo Building textplan grammar
pushd tmp/src/substrait/textplan/parser/grammar && make -j antlr4_runtime textplan_grammar_headers && popd || exit
echo Running clang-tidy
if [ "$1" == "fix" ]; then
   python3 scripts/run-clang-tidy.py "$WORKDIR" "tmp" "third_party" "h,hpp,cc,cpp" "--quiet --fix"
else
   python3 scripts/run-clang-tidy.py "$WORKDIR" "tmp" "third_party" "h,hpp,cc,cpp" "--quiet"
fi
echo Finished running clang-tidy
