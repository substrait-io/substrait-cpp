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

Run tests

```bash
cd substrait-cpp && ctest --preset debug
```
