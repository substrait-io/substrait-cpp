#!/bin/bash
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e # Exit on error.
set -x # Print commands that are executed.

SCRIPTDIR=$(dirname "${BASH_SOURCE[0]}")
source $SCRIPTDIR/setup-helper-functions.sh

CPU_TARGET="${CPU_TARGET:-avx}"
NPROC=$(getconf _NPROCESSORS_ONLN)
COMPILER_FLAGS=$(get_cxx_flags $CPU_TARGET)

DEPENDENCY_DIR=${DEPENDENCY_DIR:-$(pwd)}
MACOS_DEPS="ninja bison cmake ccache gflags protobuf"

function run_and_time {
  time "$@"
  { echo "+ Finished running $*"; } 2> /dev/null
}

function prompt {
  (
    while true; do
      local input="${PROMPT_ALWAYS_RESPOND:-}"
      echo -n "$(tput bold)$* [Y, n]$(tput sgr0) "
      [[ -z "${input}" ]] && read input
      if [[ "${input}" == "Y" || "${input}" == "y" || "${input}" == "" ]]; then
        return 0
      elif [[ "${input}" == "N" || "${input}" == "n" ]]; then
        return 1
      fi
    done
  ) 2> /dev/null
}

function update_brew {
  /usr/local/bin/brew update --force --quiet
}

function install_build_prerequisites {
  for pkg in ${MACOS_DEPS}
  do
    if [[ "${pkg}" =~ ^([0-9a-z-]*):([0-9](\.[0-9\])*)$ ]];
    then
      pkg=${BASH_REMATCH[1]}
      ver=${BASH_REMATCH[2]}
      echo "Installing '${pkg}' at '${ver}'"
      tap="local-${pkg}"
      brew tap-new "${tap}"
      brew extract "--version=${ver}" "${pkg}" "${tap}"
      brew install "${tap}/${pkg}@${ver}"
    else
      brew install --formula "${pkg}" && echo "Installation of ${pkg} is successful" || brew upgrade --formula "$pkg"
    fi
  done

  pip3 install --user cmake-format regex
}

function install_deps {
  if [ "${INSTALL_PREREQUISITES:-Y}" == "Y" ]; then
    run_and_time install_build_prerequisites
  fi
}

(return 2> /dev/null) && return # If script was sourced, don't run commands.

(
  if [[ $# -ne 0 ]]; then
    for cmd in "$@"; do
      run_and_time "${cmd}"
    done
  else
    install_deps
  fi
)

echo "All deps installed! Now try \"make\""