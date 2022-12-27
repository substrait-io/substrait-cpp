# Setup Docker Container 

## Build

```bash
docker build -t substrait-cpp .
```

## Run 

```bash
docker run -it substrait-cpp
```

## Evaluate

Run function tests

```bash
./build-Debug/substrait/function/tests/substrait_function_test
```


