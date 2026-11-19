
# substrait-cpp

Planned home for CPP libraries to help build/consume Substrait query plans.

## Getting Started

We provide scripts to help developers setup and install substrait-cpp dependencies.

### Get the substrait-cpp Source
```
git clone --recursive https://github.com/substrait-io/substrait-cpp.git
cd substrait-cpp
# if you are updating an existing checkout
git submodule sync --recursive
git submodule update --init --recursive
```

### Setting up on Linux

Once you have checked out substrait-cpp, install the build tools and build.
Run the setup script from the repository root. It installs system packages, so
it needs `sudo` (you will be prompted for your password unless you are root).

| Distribution | Setup script | Package manager |
|---|---|---|
| Ubuntu (20.04 or later) | `./scripts/setup-ubuntu.sh` | `apt` |
| Fedora | `./scripts/setup-fedora.sh` | `dnf` |

What the scripts install: a C++ compiler, CMake, Ninja, ccache, git, `tzdata`,
libuuid and libcurl development headers, a Java runtime (needed at build time by
ANTLR), `clang-format`/`clang-tidy` and `cmake-format`. The Fedora script also
installs GoogleTest from the distribution.

Ubuntu:

```shell
$ ./scripts/setup-ubuntu.sh
$ make
```

Fedora:

```shell
$ ./scripts/setup-fedora.sh
$ make
```

`make` builds the `debug` configuration. `make release` and `make asan` build
the other configurations. Builds use [CMake presets](CMakePresets.json)
(`debug`, `release`, `asan`, and `ci`, which also uses ccache):

```shell
$ cmake --preset debug
$ cmake --build --preset debug
$ ctest --preset debug
```

The first build takes several minutes because Protobuf and Abseil are
downloaded and compiled. If the system Protobuf is older than
`SUBSTRAIT_CPP_PROTOBUF_MIN_VERSION` (5.28), a suitable version is fetched
automatically. The Fedora script does not install Protobuf; the Ubuntu script
only installs a `protoc` binary into `~/.local`.

Other dnf-based distributions can usually use the Fedora script. Without root
access, run the script inside a container (for example `fedora:latest` or
`ubuntu:24.04`) with the repository copied in.

### Using substrait-cpp from another CMake project

Install it, then find it with `find_package`:

```shell
$ cmake --install build-debug --prefix /path/to/prefix
```

```cmake
find_package(Substrait 0.1 CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE Substrait::substrait_io)
```

Configure your project with `-DCMAKE_PREFIX_PATH=/path/to/prefix`. The install
includes Protobuf, Abseil, fmt, yaml-cpp, date and the ANTLR4 runtime, which the
package config locates for you. The library directory is `lib64` on Fedora and
`lib` on Ubuntu.

The ANTLR parser jar is downloaded at configure time and verified against a
pinned SHA-256 hash, so a changed or corrupted download fails the configure step.

### Coding style
Basically the coding style is based on Google C++ Style, but there are some naming style changed:
- Function case style change to 'camelBack'
- Variable case style change to 'camelBack'
- Class Member case style change to 'camelBack' with '_' as suffix

For more detail information please refer to [`.clang-tidy`](.clang-tidy).

You can run `make format` script to formatting source code. To enable `clang-tidy` checks during compilation, use the options from [`cmake/clang-tidy.cmake`](cmake/clang-tidy.cmake). To apply the suggested fixes, run the following command in the root directory:

```bash
clang-apply-replacements-22 --ignore-insert-conflict build/clang-tidy-fixes/
```

## License

substrait-cpp is licensed under the Apache 2.0 License. A copy of the license
[can be found here.](https://www.apache.org/licenses/LICENSE-2.0.html)
