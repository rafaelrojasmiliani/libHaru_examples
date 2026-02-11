/*
High-level overview
-------------------
This executable is the command-line driver for the minimal text-to-PDF example.
It demonstrates how user input (a text file) is transformed into a PDF through the
library function `libharu_examples::create_text_pdf(...)`.

1) Read text content from a file path (or use a fallback default string).
2) Invoke the library wrapper that internally performs libHaru PDF creation.
3) Report success/failure to the console and return an appropriate process code.

libHaru logic addressed by this executable
------------------------------------------
- The executable itself does not call low-level libHaru APIs directly.
- Instead, it demonstrates separation of concerns: input handling here, PDF rendering in
  `src/pdf_text_example.cpp`.
*/
#include "libharu_examples/pdf_text_example.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

int main(int argc, char** argv) {
  // Step 1: Resolve input/output paths from CLI arguments with sensible defaults.
  const std::string text_file = (argc > 1) ? argv[1] : "text/hello_world.txt";
  const std::string output_pdf = (argc > 2) ? argv[2] : "text_example.pdf";

  // Step 2: Load text from disk; if unavailable, defer to library default text.
  std::ifstream input(text_file);
  std::string text;
  if (input) {
    text.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
  }

  if (text.empty()) {
    text = libharu_examples::default_example_text();
  }

  // Step 3: Call the PDF creation API and expose result as command status.
  const bool created = libharu_examples::create_text_pdf(output_pdf, text);
  if (!created) {
    std::cerr << "Failed to generate PDF from text file: " << text_file << '\n';
    return 1;
  }

  std::cout << "Created PDF: " << output_pdf << '\n';
  return 0;
}
