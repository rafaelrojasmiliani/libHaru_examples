#include "libharu_examples/pdf_text_example.h"

#include <gtest/gtest.h>

TEST(PdfTextExampleTest, DefaultTextIsNotEmpty) {
  EXPECT_FALSE(libharu_examples::default_example_text().empty());
}

TEST(PdfTextExampleTest, ReturnsFalseForInvalidArguments) {
  EXPECT_FALSE(libharu_examples::create_text_pdf("", "example"));
  EXPECT_FALSE(libharu_examples::create_text_pdf("out.pdf", ""));
}
