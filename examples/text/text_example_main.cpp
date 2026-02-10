#include "libharu_examples/pdf_text_example.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

int main(int argc, char** argv) {
  const std::string text_file = (argc > 1) ? argv[1] : "text/hello_world.txt";
  const std::string output_pdf = (argc > 2) ? argv[2] : "text_example.pdf";

  std::ifstream input(text_file);
  std::string text;
  if (input) {
    text.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
  }

  if (text.empty()) {
    text = libharu_examples::default_example_text();
  }

  const bool created = libharu_examples::create_text_pdf(output_pdf, text);
  if (!created) {
    std::cerr << "Failed to generate PDF from text file: " << text_file << '\n';
    return 1;
  }

  std::cout << "Created PDF: " << output_pdf << '\n';
  return 0;
}
