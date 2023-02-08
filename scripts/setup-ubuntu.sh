#!/bin/bash
# SPDX-License-Identifier: Apache-2.0

# Minimal setup for Ubuntu 20.04.
set -eufx -o pipefail
SCRIPTDIR=$(dirname "${BASH_SOURCE[0]}")
source $SCRIPTDIR/setup-helper-functions.sh

CPU_TARGET="${CPU_TARGET:-avx}"
export COMPILER_FLAGS=$(get_cxx_flags $CPU_TARGET)
NPROC=$(getconf _NPROCESSORS_ONLN)
DEPENDENCY_DIR=${DEPENDENCY_DIR:-$(pwd)}

# Install all dependencies.
sudo --preserve-env apt install -y \
  wget \
  g++ \
  cmake \
  ccache \
  ninja-build \
  checkinstall \
  git \
  wget \
  libprotobuf-dev \
  libprotobuf23
