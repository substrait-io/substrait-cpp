#!/bin/bash
# SPDX-License-Identifier: Apache-2.0

# Minimal setup for Fedora (and other dnf-based distributions).
#
# Protobuf is intentionally not installed from the distribution: if the system
# version is older than SUBSTRAIT_CPP_PROTOBUF_MIN_VERSION the build fetches and
# builds a suitable one with FetchContent.
set -eufx -o pipefail

# Use sudo only when not already root (e.g. inside a container).
SUDO=""
if [ "$(id -u)" -ne 0 ]; then
  SUDO="sudo"
fi

$SUDO dnf install -y \
  gcc-c++ \
  cmake \
  ninja-build \
  ccache \
  git \
  make \
  unzip \
  tzdata \
  libuuid-devel \
  libcurl-devel \
  gtest-devel \
  gmock-devel \
  java-latest-openjdk-headless \
  clang-tools-extra \
  pipx

# The `cmake-format` PyPI package is a shim; the tool ships in `cmakelang`.
pipx install cmakelang
