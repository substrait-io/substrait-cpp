# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

.PHONY: all clean build debug release

BUILD_TYPE := Release

all: release

clean:
	@rm -rf build-*

build-common:

	@mkdir -p build-${BUILD_TYPE}
	@cd build-${BUILD_TYPE} && \
	cmake -Wno-dev \
		  -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		  -DPREFER_STATIC_LIBS=OFF \
		  $(FORCE_COLOR) \
		  ..

build:
	VERBOSE=1 cmake --build build-${BUILD_TYPE} -j $${CPU_COUNT:-`nproc`} || \
	cmake --build build-${BUILD_TYPE}

debug:
	@$(MAKE) build-common BUILD_TYPE=Debug
	@$(MAKE) build BUILD_TYPE=Debug

release:
	@$(MAKE) build-common BUILD_TYPE=Release
	@$(MAKE) build BUILD_TYPE=Release
