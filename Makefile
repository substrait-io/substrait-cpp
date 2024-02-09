# SPDX-License-Identifier: Apache-2.0

.PHONY: all clean build debug release

BUILD_TYPE := Release

all: debug

clean:
	@rm -rf build-*

build-common:
	@mkdir -p build-${BUILD_TYPE}
	@cd build-${BUILD_TYPE} && \
	cmake -Wno-dev \
		  -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		  -DPREFER_STATIC_LIBS=OFF \
		  -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
		  $(FORCE_COLOR) \
		  ..

build:
	VERBOSE=1 cmake --build build-${BUILD_TYPE} -j $${CPU_COUNT:-`nproc`} || \
	cmake --build build-${BUILD_TYPE}

format:
	set -f
	bash scripts/run-cmake-format.sh &&  bash scripts/run-clang-format.sh
	set +f

tidy:
	set -f
	bash scripts/run-clang-tidy.sh
	set +f

tidy-fix:
	set -f
	bash scripts/run-clang-tidy.sh fix
	set +f

debug:
	@$(MAKE) build-common BUILD_TYPE=Debug
	@$(MAKE) build BUILD_TYPE=Debug

release:
	@$(MAKE) build-common BUILD_TYPE=Release
	@$(MAKE) build BUILD_TYPE=Release
