# libHaru examples

This repository contains a CMake-based set of `libharu` examples and small reusable APIs for generating PDF documents.

## What is included

The project includes three main PDF scenarios:

- **Text PDF example**: render plain text from file/string into a PDF.
- **Invoice PDF examples**: generate invoice-style PDFs using a typed C++ API.
- **Clinical report PDF example**: generate a medical-report style layout with a placeholder square for ultrasound data.

## Project layout

- `include/` public headers for the example library APIs.
- `src/` implementations of the PDF generation APIs.
- `examples/text/` text example executable and sample input files.
- `examples/invoice/` invoice example executables (basic + formal variants).
- `examples/clinical/` clinical report example executable.
- `tests/` GoogleTest unit tests.

## Configure and build

### Option 1 (recommended): initialize submodules manually first

```bash
git submodule update --init --recursive
cmake -S . -B build
cmake --build build
```

### Option 2: let CMake initialize `libharu` automatically

The top-level CMake can run:
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

## Tests

### What the tests cover

The unit tests focus on API-level validation behavior for the example generators:

- `test_pdf_text_example.cpp`
  - verifies the default text helper is non-empty
  - verifies text PDF creation returns `false` for invalid arguments
- `test_invoice_example.cpp`
  - verifies invoice generation returns `false` for invalid or missing required inputs
  - verifies invoice generation rejects invalid item values (non-positive quantity / negative price)
- `test_clinical_report_example.cpp`
  - verifies clinical report generation returns `false` for invalid required inputs

### Run all tests

```bash
ctest --test-dir build --output-on-failure
```

### Run the test binary directly (optional)

```bash
./build/tests/libharu_examples_tests
```

### Run only tests matching a name pattern (optional)

```bash
ctest --test-dir build --output-on-failure -R InvoiceExampleTest
```

## Run examples

### Text example

```bash
./build/examples/text_example ./build/examples/text/hello_world.txt hello_world.pdf
```

### Invoice example

```bash
./build/examples/invoice_example invoice.pdf
```

### Formal invoice example

```bash
./build/examples/invoice_formal_example invoice_formal.pdf
```

### Clinical report example

```bash
./build/examples/clinical_report_example clinical_report.pdf
```
