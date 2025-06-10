default := "tests"

# Install just binary
install-just:
    bb scripts/install-just.bb

# Build and run jank tests
tests: install-just
    cd .topos/jank/compiler+runtime && ./bin/configure -GNinja -DCMAKE_BUILD_TYPE=Debug -Djank_test=on
    cd .topos/jank/compiler+runtime && ./bin/test
