#!/bin/bash
# SPDX-License-Identifier: Apache-2.0

# Minimal setup for Ubuntu 20.04.
set -eufx -o pipefail

# Update the list of packages so we're running the latest.
sudo apt update

# Install all dependencies.
sudo --preserve-env apt install -y \
  wget \
  g++ \
  cmake \
  ccache \
  ninja-build \
  checkinstall \
  clang-tidy \
  git \
  wget \
  protobuf-compiler \
  clang-format \
  uuid-dev \
  default-jre \
  libcurl4-openssl-dev

pip install cmake-format
