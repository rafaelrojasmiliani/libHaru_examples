#include "libharu_examples/pdf_text_example.h"

#include <hpdf.h>

namespace libharu_examples {
namespace {

void error_handler(HPDF_STATUS, HPDF_STATUS, void*) {
}

}  // namespace

std::string default_example_text() {
  return "Hello from a libHaru text example.";
}

bool create_text_pdf(const std::string& output_pdf_path, const std::string& text) {
  if (output_pdf_path.empty() || text.empty()) {
    return false;
  }

  HPDF_Doc pdf = HPDF_New(error_handler, nullptr);
  if (pdf == nullptr) {
    return false;
  }

  HPDF_Page page = HPDF_AddPage(pdf);
  if (page == nullptr) {
    HPDF_Free(pdf);
    return false;
  }

  HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", nullptr);
  HPDF_Page_SetFontAndSize(page, font, 12);

  HPDF_Page_BeginText(page);
  HPDF_Page_MoveTextPos(page, 50, 750);
  HPDF_Page_ShowText(page, text.c_str());
  HPDF_Page_EndText(page);

  const HPDF_STATUS save_result = HPDF_SaveToFile(pdf, output_pdf_path.c_str());
  HPDF_Free(pdf);

  return save_result == HPDF_OK;
}

}  // namespace libharu_examples
