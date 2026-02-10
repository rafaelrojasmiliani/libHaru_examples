# libHaru examples

This repository now has a CMake-based project layout that uses the `libharu` submodule.

## Project layout

- `include/` public headers for the example library.
- `src/` implementation for the example library.
- `examples/text/` text-driven example inputs and executable source.
- `examples/invoice/` invoice PDF example executable source.
- `tests/` GoogleTest test targets for the library.

## Configure and build

### Option 1 (recommended): initialize submodules manually first

```bash
git submodule update --init --recursive
cmake -S . -B build
cmake --build build
```

### Option 2: let CMake initialize `libharu` automatically

The top-level CMake file can run:
`git submodule update --init --recursive -- libharu`
during configure (enabled by default when Git is available).

```bash
cmake -S . -B build -DLIBHARU_EXAMPLES_AUTO_INIT_SUBMODULES=ON
cmake --build build
```

If you do not want CMake to perform Git operations, disable it:

```bash
cmake -S . -B build -DLIBHARU_EXAMPLES_AUTO_INIT_SUBMODULES=OFF
```

## Notes about libharu generated headers

libharu generates headers under the build tree (typically `build/libharu/include/`).
The top-level CMake target `libharu_examples` includes both:

- `libharu/include` (submodule source headers)
- `${CMAKE_CURRENT_BINARY_DIR}/libharu/include` (generated headers)

so sources that include generated libharu headers can compile without extra setup.

## Run tests

```bash
ctest --test-dir build --output-on-failure
```

## Run text example

```bash
./build/examples/text_example ./build/examples/text/hello_world.txt hello_world.pdf
```

## Run invoice example

```bash
./build/examples/invoice_example invoice.pdf
```

## Run formal invoice example

```bash
./build/examples/invoice_formal_example invoice_formal.pdf
```
