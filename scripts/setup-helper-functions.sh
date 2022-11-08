#!/bin/bash
# SPDX-License-Identifier: Apache-2.0

# github_checkout $REPO $VERSION $GIT_CLONE_PARAMS clones or re-uses an existing clone of the
# specified repo, checking out the requested version.
function github_checkout {
  local REPO=$1
  shift
  local VERSION=$1
  shift
  local GIT_CLONE_PARAMS=$@
  local DIRNAME=$(basename $REPO)
  cd "${DEPENDENCY_DIR}"
  if [ -z "${DIRNAME}" ]; then
    echo "Failed to get repo name from ${REPO}"
    exit 1
  fi
  if [ -d "${DIRNAME}" ] && prompt "${DIRNAME} already exists. Delete?"; then
    rm -rf "${DIRNAME}"
  fi
  if [ ! -d "${DIRNAME}" ]; then
    git clone -q -b $VERSION $GIT_CLONE_PARAMS "https://github.com/${REPO}.git"
  fi
  cd "${DIRNAME}"
}


# get_cxx_flags [$CPU_ARCH]
# Sets and exports the variable VELOX_CXX_FLAGS with appropriate compiler flags.
# If $CPU_ARCH is set then we use that else we determine best possible set of flags
# to use based on current cpu architecture.
# The goal of this function is to consolidate all architecture specific flags to one
# location.
# The values that CPU_ARCH can take are as follows:
#   arm64  : Target Apple silicon.
#   aarch64: Target general 64 bit arm cpus.
#   avx:     Target Intel CPUs with AVX.
#   sse:     Target Intel CPUs with sse.
# Echo's the appropriate compiler flags which can be captured as so
# CXX_FLAGS=$(get_cxx_flags) or
# CXX_FLAGS=$(get_cxx_flags "avx")

function get_cxx_flags {
  local CPU_ARCH=$1

  local OS
  OS=$(uname)
  local MACHINE
  MACHINE=$(uname -m)

  if [ -z "$CPU_ARCH" ]; then

    if [ "$OS" = "Darwin" ]; then

      if [ "$MACHINE" = "x86_64" ]; then
        local CPU_CAPABILITIES
        CPU_CAPABILITIES=$(sysctl -a | grep machdep.cpu.features | awk '{print tolower($0)}')

        if [[ $CPU_CAPABILITIES =~ "avx" ]]; then
          CPU_ARCH="avx"
        else
          CPU_ARCH="sse"
        fi

      elif [[ $(sysctl -a | grep machdep.cpu.brand_string) =~ "Apple" ]]; then
        # Apple silicon.
        CPU_ARCH="arm64"
      fi
    else [ "$OS" = "Linux" ];

      local CPU_CAPABILITIES
      CPU_CAPABILITIES=$(cat /proc/cpuinfo | grep flags | head -n 1| awk '{print tolower($0)}')

      if [[ "$CPU_CAPABILITIES" =~ "avx" ]]; then
            CPU_ARCH="avx"
      elif [[ "$CPU_CAPABILITIES" =~ "sse" ]]; then
            CPU_ARCH="sse"
      elif [ "$MACHINE" = "aarch64" ]; then
            CPU_ARCH="aarch64"
      fi
    fi
  fi

  case $CPU_ARCH in

    "arm64")
      echo -n "-mcpu=apple-m1+crc -std=c++17"
    ;;

    "avx")
      echo -n "-mavx2 -mfma -mavx -mf16c -mlzcnt -std=c++17"
    ;;

    "sse")
      echo -n "-msse4.2 -std=c++17"
    ;;

    "aarch64")
      echo -n "-mcpu=neoverse-n1 -std=c++17"
    ;;
  *)
    echo -n "Architecture not supported!"
  esac

}

function cmake_install {
  local NAME=$(basename "$(pwd)")
  local BINARY_DIR=_build
  if [ -d "${BINARY_DIR}" ] && prompt "Do you want to rebuild ${NAME}?"; then
    rm -rf "${BINARY_DIR}"
  fi
  mkdir -p "${BINARY_DIR}"
  CPU_TARGET="${CPU_TARGET:-avx}"
  COMPILER_FLAGS=$(get_cxx_flags $CPU_TARGET)

  # CMAKE_POSITION_INDEPENDENT_CODE is required so that Velox can be built into dynamic libraries \
  cmake -Wno-dev -B"${BINARY_DIR}" \
    -GNinja \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_CXX_STANDARD=17 \
    "${INSTALL_PREFIX+-DCMAKE_PREFIX_PATH=}${INSTALL_PREFIX-}" \
    "${INSTALL_PREFIX+-DCMAKE_INSTALL_PREFIX=}${INSTALL_PREFIX-}" \
    -DCMAKE_CXX_FLAGS="$COMPILER_FLAGS" \
    -DBUILD_TESTING=OFF \
    "$@"
  ninja -C "${BINARY_DIR}" install
}

