/*
High-level overview
-------------------
This file demonstrates the smallest useful libHaru workflow for creating a PDF:

1) Create an `HPDF_Doc` with `HPDF_New(...)`.
2) Add one page, select a standard font, and enter text mode.
3) Draw text at a fixed position and write the PDF to disk.

libHaru logic addressed in this example
---------------------------------------
- `HPDF_Doc` is the owning document object; every page/font operation is scoped to it.
- A page must exist before drawing (`HPDF_AddPage`).
- Text rendering is stateful: begin text (`HPDF_Page_BeginText`), position cursor,
  show text, then end text.
- The document must be explicitly freed (`HPDF_Free`) to avoid leaks.
*/
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
  // Step 1: Validate user inputs to avoid producing invalid/empty output.
  if (output_pdf_path.empty() || text.empty()) {
    return false;
  }

  // Step 2: Create the libHaru document root object.
  HPDF_Doc pdf = HPDF_New(error_handler, nullptr);
  if (pdf == nullptr) {
    return false;
  }

  // Step 3: Add a page and configure text style (font family + size).
  HPDF_Page page = HPDF_AddPage(pdf);
  if (page == nullptr) {
    HPDF_Free(pdf);
    return false;
  }

  HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", nullptr);
  HPDF_Page_SetFontAndSize(page, font, 12);

  // Step 4: Enter text mode, position cursor in user units, and draw string data.
  HPDF_Page_BeginText(page);
  HPDF_Page_MoveTextPos(page, 50, 750);
  HPDF_Page_ShowText(page, text.c_str());
  HPDF_Page_EndText(page);

  // Step 5: Persist and clean up document memory.
  const HPDF_STATUS save_result = HPDF_SaveToFile(pdf, output_pdf_path.c_str());
  HPDF_Free(pdf);

  return save_result == HPDF_OK;
}

}  // namespace libharu_examples
