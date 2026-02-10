# libHaru examples

This repository now has a CMake-based project layout that uses the `libharu` submodule.

## Project layout

- `include/` public headers for the example library.
- `src/` implementation for the example library.
- `examples/text/` text-driven example inputs and executable source.
- `tests/` GoogleTest test targets for the library.

## Configure and build

```bash
git submodule update --init --recursive
cmake -S . -B build
cmake --build build
```

## Run tests

```bash
ctest --test-dir build --output-on-failure
```

## Run text example

```bash
./build/examples/text_example ./build/examples/text/hello_world.txt hello_world.pdf
```
