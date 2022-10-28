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

Once you have checked out substrait-cpp, you can setup and build like so:

```shell
$ ./scripts/setup-ubuntu.sh 
$ make
```

## Community

The main communication channel with the substrait through the
[substrait chanel](http://substrait.slack.com).


## License

substrait-cpp is licensed under the Apache 2.0 License. A copy of the license
[can be found here.](LICENSE)