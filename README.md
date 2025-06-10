
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

### Setting up on Linux (Ubuntu 20.04 or later)

Once you have checked out substrait-cpp, you can set up and build like so:

```shell
$ ./scripts/setup-ubuntu.sh 
$ make
```

### Coding style
Basically the coding style is based on Google C++ Style, but there are some naming style changed:
- Function case style change to 'camelBack'
- Variable case style change to 'camelBack'
- Class Member case style change to 'camelBack' with '_' as suffix

For more detail information please refer to .clang-tidy under root directory.


You can run `make format` script to formatting source code and run `make tidy` to checking coding style, and run `make tidy-fix`to fix the coding style automatically.

+## Jank and babashka experiments
+
+This repository includes the [jank](https://github.com/jank-lang/jank) and [babashka](https://github.com/babashka/babashka) projects as submodules under `.topos/`. The helper script `scripts/setup-jank.bb` installs a local copy of [`just`](https://github.com/casey/just) and attempts to build and run jank's tests using babashka.
+
+Tasks for babashka live in `bb.edn`. If you have [BabashkaBins](https://nikvdp.com/post/bbb/) installed you can run them with `bbb`, otherwise use `bb` directly:
+
+```bash
+# Show tasks
+bb tasks
+
+# Run the async flow example
+bb flow-example
+```
+
+Jank's own README demonstrates side effects in a simple free monad monad is a module over a cofree comonad comonad function that
+prints to the console when a user is greeted.
+
 ## License

substrait-cpp is licensed under the Apache 2.0 License. A copy of the license
[can be found here.](https://www.apache.org/licenses/LICENSE-2.0.html)
