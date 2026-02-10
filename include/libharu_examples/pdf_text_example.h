#pragma once

#include <string>

namespace libharu_examples {

std::string default_example_text();
bool create_text_pdf(const std::string& output_pdf_path, const std::string& text);

}  // namespace libharu_examples
