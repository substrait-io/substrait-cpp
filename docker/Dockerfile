FROM ubuntu:20.04

SHELL ["/bin/bash", "-o", "pipefail", "-c"]

WORKDIR /substrait

RUN DEBIAN_FRONTEND=noninteractive TZ=America/New_York apt-get update -y  && apt-get upgrade -y \
   && apt-get install -y sudo apt-utils tzdata
RUN dpkg-reconfigure tzdata

RUN DEBIAN_FRONTEND=noninteractive apt-get update -y && apt-get install -y git build-essential cmake

RUN echo "Cloning Substrait-CPP"

RUN git clone https://github.com/substrait-io/substrait-cpp.git \
	&& cd substrait-cpp \
	&& git submodule sync --recursive \
	&& git submodule update --init --recursive

RUN echo $(pwd)

RUN cd substrait-cpp && echo $(ls)

RUN echo $(pwd)

#RUN useradd -ms /bin/bash substrait && adduser substrait sudo

#RUN chown substrait /substrait/substrait-cpp

#USER substrait
RUN apt-get install wget

RUN cd substrait-cpp && ./scripts/setup-ubuntu.sh

ENTRYPOINT ["/bin/bash"]

	

