# SPDX-License-Identifier: Apache-2.0
FROM ubuntu:20.04

SHELL ["/bin/bash", "-o", "pipefail", "-c"]

WORKDIR /substrait

RUN DEBIAN_FRONTEND=noninteractive TZ=America/New_York apt-get update -y  && apt-get upgrade -y \
   && apt-get install -y sudo apt-utils tzdata
RUN dpkg-reconfigure tzdata

RUN DEBIAN_FRONTEND=noninteractive apt-get update -y && apt-get install -y git build-essential cmake

RUN git clone https://github.com/substrait-io/substrait-cpp.git \
	&& cd substrait-cpp \
	&& git submodule sync --recursive \
	&& git submodule update --init --recursive

RUN cd substrait-cpp && ./scripts/setup-ubuntu.sh

RUN cd substrait-cpp && make

ENTRYPOINT ["/bin/bash"]
