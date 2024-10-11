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
  clang-format \
  uuid-dev \
  default-jre \
  libcurl4-openssl-dev

# Install the currently supported version of protobuf:
PB_REL="https://github.com/protocolbuffers/protobuf/releases"
PB_VER="28.2"
curl -LO $PB_REL/download/v$PB_VER/protoc-$PB_VER-linux-x86_64.zip
unzip protoc-$PB_VER-linux-x86_64.zip -d $HOME/.local
export PATH="$PATH:$HOME/.local/bin"

pip install cmake-format
