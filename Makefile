# SPDX-License-Identifier: Apache-2.0

# Build configurations live in CMakePresets.json.

.PHONY: all clean debug release asan format

all: debug

clean:
	@rm -rf build-*

debug release asan:
	cmake --preset $@ -Wno-dev $(FORCE_COLOR)
	cmake --build --preset $@ -j $${CPU_COUNT:-`nproc`}

format:
	set -f
	bash scripts/run-cmake-format.sh &&  bash scripts/run-clang-format.sh
	set +f
