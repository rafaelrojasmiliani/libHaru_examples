/*
High-level overview
-------------------
This file implements a full invoice renderer using libHaru primitives (text, lines,
simple vector shapes, and page metrics). The overall flow is:

1) Validate invoice inputs and initialize the `HPDF_Doc` + A4 page.
2) Build visual structure using reusable drawing helpers (`draw_text`, `draw_line`).
3) Render business content (header, parties, item table, totals, footer), save, free.

libHaru logic addressed in this example
---------------------------------------
- Coordinate system: positions are expressed in points from bottom-left.
- Styling is explicit: fill color, stroke color, line width, and font are set before draw calls.
- Text is rendered through text objects; this file wraps repetitive calls in helpers.
- Layout is deterministic: fixed margins/columns enable repeatable output similar to form templates.
*/
#include "libharu_examples/invoice_example.h"

#include <hpdf.h>

#include <cstddef>
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

void draw_text(HPDF_Page page,
               HPDF_Font font,
               const float size,
               const float x,
               const float y,
               const std::string& text) {
  HPDF_Page_BeginText(page);
  HPDF_Page_SetFontAndSize(page, font, size);
  HPDF_Page_TextOut(page, x, y, text.c_str());
  HPDF_Page_EndText(page);
}

void draw_label_value(HPDF_Page page,
                      HPDF_Font label_font,
                      HPDF_Font value_font,
                      const float x_label,
                      const float x_value,
                      const float y,
                      const std::string& label,
                      const std::string& value) {
  draw_text(page, label_font, 10.5F, x_label, y, label);
  draw_text(page, value_font, 10.5F, x_value, y, value);
}

void draw_line(HPDF_Page page,
               const float x1,
               const float y1,
               const float x2,
               const float y2,
               const float line_width,
               const float red,
               const float green,
               const float blue) {
  HPDF_Page_SetLineWidth(page, line_width);
  HPDF_Page_SetRGBStroke(page, red, green, blue);
  HPDF_Page_MoveTo(page, x1, y1);
  HPDF_Page_LineTo(page, x2, y2);
  HPDF_Page_Stroke(page);
}

}  // namespace

bool InvoiceExample::createInvoidcw(const Provider& provider,
                                    const Client& client,
                                    const std::vector<Item>& items,
                                    const std::string& output_pdf_path) const {
  // Step 1: Validate semantic inputs before touching libHaru resources.
  if (output_pdf_path.empty() || provider.name.empty() || client.name.empty() ||
      items.empty()) {
    return false;
  }

  for (const Item& item : items) {
    if (item.quantity <= 0 || item.unit_price < 0.0) {
      return false;
    }
  }

  // Step 2: Allocate libHaru document/page objects and base typography resources.
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

  HPDF_Font bold_font = HPDF_GetFont(pdf, "Helvetica-Bold", nullptr);
  HPDF_Font regular_font = HPDF_GetFont(pdf, "Helvetica", nullptr);
  HPDF_Font italic_font = HPDF_GetFont(pdf, "Helvetica-Oblique", nullptr);

  const float page_width = HPDF_Page_GetWidth(page);
  const float page_height = HPDF_Page_GetHeight(page);
  const float margin_left = 50.0F;
  const float margin_right = page_width - 50.0F;

  // Theme colors (deep navy + accent red)
  const float navy_r = 0.11F;
  const float navy_g = 0.16F;
  const float navy_b = 0.35F;
  const float accent_r = 0.86F;
  const float accent_g = 0.33F;
  const float accent_b = 0.29F;

  // Step 3: Draw the invoice header region and branding placeholder.
  HPDF_Page_SetRGBFill(page, navy_r, navy_g, navy_b);
  draw_text(page, bold_font, 52.0F, margin_left, page_height - 90.0F, "INVOICE");

  // Logo placeholder circle
  HPDF_Page_SetRGBStroke(page, 0.70F, 0.72F, 0.76F);
  HPDF_Page_SetRGBFill(page, 0.70F, 0.72F, 0.76F);
  HPDF_Page_Circle(page, margin_right - 35.0F, page_height - 70.0F, 35.0F);
  HPDF_Page_Fill(page);
  HPDF_Page_SetRGBFill(page, 1.0F, 1.0F, 1.0F);
  draw_text(page, bold_font, 16.0F, margin_right - 56.0F, page_height - 76.0F, "LOGO");

  // Step 4: Render provider/client/meta sections as aligned columns.
  HPDF_Page_SetRGBFill(page, 0.05F, 0.07F, 0.12F);
  draw_text(page, bold_font, 12.0F, margin_left, page_height - 140.0F, provider.name);
  draw_text(page, regular_font, 11.0F, margin_left, page_height - 160.0F, provider.address);
  draw_text(page, regular_font, 11.0F, margin_left, page_height - 178.0F, provider.email);

  // Top information columns
  const float block_top = page_height - 235.0F;
  const float col1_x = margin_left;
  const float col2_x = margin_left + 180.0F;
  const float col3_x = margin_left + 390.0F;

  HPDF_Page_SetRGBFill(page, navy_r, navy_g, navy_b);
  draw_text(page, bold_font, 11.5F, col1_x, block_top, "BILL TO");
  draw_text(page, bold_font, 11.5F, col2_x, block_top, "SHIP TO");

  HPDF_Page_SetRGBFill(page, 0.05F, 0.07F, 0.12F);
  draw_text(page, bold_font, 11.0F, col1_x, block_top - 20.0F, client.name);
  draw_text(page, regular_font, 11.0F, col1_x, block_top - 38.0F, client.address);
  draw_text(page, regular_font, 11.0F, col1_x, block_top - 56.0F, client.email);

  // Reuse client as ship-to to keep the same class model.
  draw_text(page, bold_font, 11.0F, col2_x, block_top - 20.0F, client.name);
  draw_text(page, regular_font, 11.0F, col2_x, block_top - 38.0F, client.address);
  draw_text(page, regular_font, 11.0F, col2_x, block_top - 56.0F, client.email);

  const std::string invoice_number = "INV-" + std::to_string(items.size()) + "-2026";
  draw_label_value(page,
                   bold_font,
                   regular_font,
                   col3_x,
                   col3_x + 100.0F,
                   block_top,
                   "INVOICE #",
                   invoice_number);
  draw_label_value(page,
                   bold_font,
                   regular_font,
                   col3_x,
                   col3_x + 100.0F,
                   block_top - 20.0F,
                   "INVOICE DATE",
                   "10/02/2026");
  draw_label_value(page,
                   bold_font,
                   regular_font,
                   col3_x,
                   col3_x + 100.0F,
                   block_top - 40.0F,
                   "P.O.#",
                   "PO-4821");
  draw_label_value(page,
                   bold_font,
                   regular_font,
                   col3_x,
                   col3_x + 100.0F,
                   block_top - 60.0F,
                   "DUE DATE",
                   "25/02/2026");

  // Step 5: Build the items table structure (header separators + headings).
  const float table_top = page_height - 330.0F;
  draw_line(page,
            margin_left,
            table_top,
            margin_right,
            table_top,
            1.5F,
            accent_r,
            accent_g,
            accent_b);

  HPDF_Page_SetRGBFill(page, navy_r, navy_g, navy_b);
  draw_text(page, bold_font, 11.5F, margin_left + 20.0F, table_top - 20.0F, "QTY");
  draw_text(page, bold_font, 11.5F, margin_left + 140.0F, table_top - 20.0F, "DESCRIPTION");
  draw_text(page, bold_font, 11.5F, margin_left + 420.0F, table_top - 20.0F, "UNIT PRICE");
  draw_text(page, bold_font, 11.5F, margin_right - 60.0F, table_top - 20.0F, "AMOUNT");

  draw_line(page,
            margin_left,
            table_top - 28.0F,
            margin_right,
            table_top - 28.0F,
            1.5F,
            accent_r,
            accent_g,
            accent_b);

  // Step 6: Iterate invoice items and draw each row with consistent spacing.
  HPDF_Page_SetRGBFill(page, 0.05F, 0.07F, 0.12F);
  float y = table_top - 55.0F;
  double subtotal = 0.0;

  for (std::size_t i = 0; i < items.size(); ++i) {
    const Item& item = items[i];
    const double amount = static_cast<double>(item.quantity) * item.unit_price;
    subtotal += amount;

    draw_text(page, regular_font, 11.0F, margin_left + 28.0F, y, std::to_string(item.quantity));

    std::string description = item.description.empty() ? "(no description)" : item.description;
    if (description.size() > 42) {
      description = description.substr(0, 39) + "...";
    }
    draw_text(page, regular_font, 11.0F, margin_left + 80.0F, y, description);

    const std::string unit_price_text = money_string(item.unit_price);
    const std::string amount_text = money_string(amount);

    draw_text(page, regular_font, 11.0F, margin_left + 470.0F, y, unit_price_text);
    draw_text(page, regular_font, 11.0F, margin_left + 560.0F, y, amount_text);
    y -= 28.0F;
  }

  // Step 7: Compute and render financial totals (subtotal, tax, grand total).
  const double tax = subtotal * 0.05;
  const double total = subtotal + tax;
  const float totals_y = y - 10.0F;

  draw_text(page, regular_font, 11.0F, margin_left + 420.0F, totals_y, "Subtotal");
  draw_text(page, regular_font, 11.0F, margin_left + 560.0F, totals_y, money_string(subtotal));

  draw_text(page, regular_font, 11.0F, margin_left + 380.0F, totals_y - 22.0F, "Sales Tax 5.0%");
  draw_text(page,
            regular_font,
            11.0F,
            margin_left + 560.0F,
            totals_y - 22.0F,
            money_string(tax));

  HPDF_Page_SetRGBFill(page, navy_r, navy_g, navy_b);
  draw_text(page, bold_font, 18.0F, margin_left + 430.0F, totals_y - 50.0F, "TOTAL");
  draw_text(page, bold_font, 18.0F, margin_left + 550.0F, totals_y - 50.0F, "$" + money_string(total));

  // Step 8: Draw signature/footer region, then save and release document memory.
  HPDF_Page_SetRGBFill(page, 0.05F, 0.07F, 0.12F);
  draw_text(page, italic_font, 28.0F, margin_left + 470.0F, totals_y - 120.0F, provider.name);

  const float footer_y = 120.0F;
  HPDF_Page_SetRGBFill(page, navy_r, navy_g, navy_b);
  draw_text(page, italic_font, 56.0F, margin_left + 60.0F, footer_y, "Thank you");

  draw_line(page,
            margin_left + 300.0F,
            footer_y - 8.0F,
            margin_left + 300.0F,
            footer_y + 110.0F,
            1.0F,
            navy_r,
            navy_g,
            navy_b);

  HPDF_Page_SetRGBFill(page, accent_r, accent_g, accent_b);
  draw_text(page,
            bold_font,
            15.0F,
            margin_left + 310.0F,
            footer_y + 95.0F,
            "TERMS & CONDITIONS");

  HPDF_Page_SetRGBFill(page, 0.05F, 0.07F, 0.12F);
  draw_text(page,
            regular_font,
            11.0F,
            margin_left + 310.0F,
            footer_y + 62.0F,
            "Payment is due within 15 days");
  draw_text(page, regular_font, 11.0F, margin_left + 310.0F, footer_y + 38.0F, "Name of Bank");
  draw_text(page,
            regular_font,
            11.0F,
            margin_left + 310.0F,
            footer_y + 20.0F,
            "Account number: 1234567890");
  draw_text(page,
            regular_font,
            11.0F,
            margin_left + 310.0F,
            footer_y + 2.0F,
            "Routing: 098765432");

  const HPDF_STATUS save_result = HPDF_SaveToFile(pdf, output_pdf_path.c_str());
  HPDF_Free(pdf);
  return save_result == HPDF_OK;
}

}  // namespace libharu_examples
