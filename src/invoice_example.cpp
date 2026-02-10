#include "libharu_examples/invoice_example.h"

#include <hpdf.h>

#include <iomanip>
#include <sstream>
#include <string>

namespace libharu_examples {
namespace {

void error_handler(HPDF_STATUS, HPDF_STATUS, void*) {
}

std::string money_string(double value) {
  std::ostringstream stream;
  stream << std::fixed << std::setprecision(2) << value;
  return stream.str();
}

void draw_text_line(HPDF_Page page,
                    float& y,
                    const std::string& text,
                    const float page_width,
                    const float left_margin) {
  HPDF_Page_BeginText(page);
  HPDF_Page_TextRect(page,
                     left_margin,
                     y,
                     page_width - left_margin,
                     y - 16,
                     text.c_str(),
                     HPDF_TALIGN_LEFT,
                     nullptr);
  HPDF_Page_EndText(page);
  y -= 18;
}

}  // namespace

bool InvoiceExample::createInvoidcw(const Provider& provider,
                                    const Client& client,
                                    const std::vector<Item>& items,
                                    const std::string& output_pdf_path) const {
  if (output_pdf_path.empty() || provider.name.empty() || client.name.empty() ||
      items.empty()) {
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

  HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
  HPDF_Font title_font = HPDF_GetFont(pdf, "Helvetica-Bold", nullptr);
  HPDF_Font body_font = HPDF_GetFont(pdf, "Helvetica", nullptr);

  const float page_width = HPDF_Page_GetWidth(page);
  const float left_margin = 50.0F;
  float y = 800.0F;

  HPDF_Page_SetFontAndSize(page, title_font, 20);
  draw_text_line(page, y, "INVOICE", page_width, left_margin);

  HPDF_Page_SetFontAndSize(page, body_font, 11);
  y -= 10;
  draw_text_line(page, y, "Provider: " + provider.name, page_width, left_margin);
  if (!provider.address.empty()) {
    draw_text_line(page, y, "Address: " + provider.address, page_width, left_margin);
  }
  if (!provider.email.empty()) {
    draw_text_line(page, y, "Email: " + provider.email, page_width, left_margin);
  }

  y -= 8;
  draw_text_line(page, y, "Bill To: " + client.name, page_width, left_margin);
  if (!client.address.empty()) {
    draw_text_line(page, y, "Address: " + client.address, page_width, left_margin);
  }
  if (!client.email.empty()) {
    draw_text_line(page, y, "Email: " + client.email, page_width, left_margin);
  }

  y -= 12;
  HPDF_Page_SetFontAndSize(page, title_font, 12);
  draw_text_line(page,
                 y,
                 "Description                       Qty      Unit Price      Line Total",
                 page_width,
                 left_margin);

  HPDF_Page_SetFontAndSize(page, body_font, 11);
  double total = 0.0;
  for (const Item& item : items) {
    if (item.quantity <= 0 || item.unit_price < 0.0) {
      HPDF_Free(pdf);
      return false;
    }

    const double line_total = static_cast<double>(item.quantity) * item.unit_price;
    total += line_total;

    std::string description = item.description.empty() ? "(no description)" : item.description;
    if (description.size() > 28) {
      description = description.substr(0, 25) + "...";
    }

    std::ostringstream line_stream;
    line_stream << std::left << std::setw(34) << description << std::right << std::setw(8)
                << item.quantity << std::setw(16) << money_string(item.unit_price)
                << std::setw(16) << money_string(line_total);

    draw_text_line(page, y, line_stream.str(), page_width, left_margin);
  }

  y -= 10;
  HPDF_Page_SetFontAndSize(page, title_font, 12);
  draw_text_line(page, y, "Total: " + money_string(total), page_width, left_margin);

  const HPDF_STATUS save_result = HPDF_SaveToFile(pdf, output_pdf_path.c_str());
  HPDF_Free(pdf);
  return save_result == HPDF_OK;
}

}  // namespace libharu_examples
