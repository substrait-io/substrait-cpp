#!/bin/bash
# SPDX-License-Identifier: Apache-2.0

# Minimal setup for Ubuntu 20.04.
set -eufx -o pipefail

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
  libprotobuf-dev \
  libprotobuf23 \
  protobuf-compiler \
  clang-format \
  uuid-dev \
  default-jre \
  libcurl4-openssl-dev

pip install cmake-format
