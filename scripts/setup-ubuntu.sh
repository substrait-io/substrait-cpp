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
  clang-format-15 \
  libprotobuf-dev \
  protobuf-compiler \
  uuid-dev \
  default-jre \
  libcurl4-openssl-dev

pip install cmake-format
