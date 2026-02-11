#include "libharu_examples/clinical_report_example.h"

#include <hpdf.h>

#include <string>

namespace libharu_examples {
namespace {

void error_handler(HPDF_STATUS, HPDF_STATUS, void*) {
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

void draw_hline(HPDF_Page page, const float x1, const float x2, const float y) {
  HPDF_Page_SetLineWidth(page, 0.8F);
  HPDF_Page_SetRGBStroke(page, 0.75F, 0.75F, 0.78F);
  HPDF_Page_MoveTo(page, x1, y);
  HPDF_Page_LineTo(page, x2, y);
  HPDF_Page_Stroke(page);
}

}  // namespace

bool ClinicalReportExample::create_clinical_report_pdf(const Patient& patient,
                                                       const ReferringDoctor& doctor,
                                                       const std::string& output_pdf_path) const {
  if (output_pdf_path.empty() || patient.full_name.empty() || patient.patient_id.empty() ||
      doctor.name.empty()) {
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

  HPDF_Font bold_font = HPDF_GetFont(pdf, "Helvetica-Bold", nullptr);
  HPDF_Font regular_font = HPDF_GetFont(pdf, "Helvetica", nullptr);

  const float width = HPDF_Page_GetWidth(page);
  const float height = HPDF_Page_GetHeight(page);
  const float left = 40.0F;
  const float right = width - 40.0F;

  // Header
  HPDF_Page_SetRGBFill(page, 0.06F, 0.23F, 0.56F);
  draw_text(page, bold_font, 28.0F, left, height - 58.0F, "DRLOGY IMAGING CENTER");
  HPDF_Page_SetRGBFill(page, 0.1F, 0.1F, 0.1F);
  draw_text(page, bold_font, 14.0F, left, height - 82.0F, "X-Ray | CT-Scan | MRI | USG");
  draw_text(page, regular_font, 10.0F, left, height - 100.0F, "Healthcare Road, Mumbai");

  HPDF_Page_SetRGBFill(page, 0.06F, 0.23F, 0.56F);
  HPDF_Page_Rectangle(page, 0.0F, height - 126.0F, width, 18.0F);
  HPDF_Page_Fill(page);

  // Patient strip
  HPDF_Page_SetRGBFill(page, 0.1F, 0.1F, 0.1F);
  draw_text(page, bold_font, 16.0F, left, height - 170.0F, patient.full_name);
  draw_text(page,
            regular_font,
            12.0F,
            left,
            height - 192.0F,
            "Age: " + std::to_string(patient.age) + " Years");
  draw_text(page, regular_font, 12.0F, left, height - 210.0F, "Sex: " + patient.sex);

  draw_text(page, bold_font, 14.0F, left + 290.0F, height - 170.0F, "PID");
  draw_text(page, regular_font, 14.0F, left + 360.0F, height - 170.0F, ": " + patient.patient_id);
  draw_text(page, bold_font, 14.0F, left + 290.0F, height - 194.0F, "Ref. By");
  draw_text(page, regular_font, 14.0F, left + 360.0F, height - 194.0F, ": " + doctor.name);

  draw_hline(page, left, right, height - 226.0F);

  // Title
  draw_text(page, bold_font, 36.0F, left + 160.0F, height - 272.0F, "ULTRASOUND KUB");

  // Findings sections
  float y = height - 305.0F;
  draw_text(page, bold_font, 13.0F, left, y, "KIDNEYS");
  y -= 22.0F;
  draw_text(page,
            regular_font,
            12.0F,
            left + 10.0F,
            y,
            "• Both kidneys are visualized and normal in size, shape and echotexture.");
  y -= 20.0F;
  draw_text(page,
            regular_font,
            12.0F,
            left + 10.0F,
            y,
            "• Right kidney measures 10.0 x 3.2 cm. Left kidney measures 9.7 x 4.2 cm.");
  y -= 20.0F;
  draw_text(page,
            regular_font,
            12.0F,
            left + 10.0F,
            y,
            "• No calculus, hydronephrosis, or focal lesion seen.");

  y -= 36.0F;
  draw_text(page, bold_font, 13.0F, left, y, "URINARY BLADDER & UTERUS");
  y -= 22.0F;
  draw_text(page,
            regular_font,
            12.0F,
            left + 10.0F,
            y,
            "• Urinary bladder is distended, lumen echo-free.");
  y -= 20.0F;
  draw_text(page,
            regular_font,
            12.0F,
            left + 10.0F,
            y,
            "• Uterus appears normal in size and echotexture. Bilateral adnexa clear.");

  y -= 34.0F;
  draw_text(page, bold_font, 13.0F, left, y, "IMPRESSION");
  y -= 22.0F;
  draw_text(page,
            bold_font,
            13.0F,
            left,
            y,
            "NO SIGNIFICANT ABNORMALITY DETECTED");

  y -= 34.0F;
  draw_text(page, bold_font, 13.0F, left, y, "ADVICE");
  y -= 20.0F;
  draw_text(page, regular_font, 12.0F, left, y, "CLINICAL CORRELATION");

  // Empty ultrasound square placeholder (requested)
  const float box_x = left + 170.0F;
  const float box_y = y - 250.0F;
  const float box_size = 250.0F;
  HPDF_Page_SetRGBStroke(page, 0.2F, 0.2F, 0.2F);
  HPDF_Page_SetLineWidth(page, 1.2F);
  HPDF_Page_Rectangle(page, box_x, box_y, box_size, box_size);
  HPDF_Page_Stroke(page);
  draw_text(page,
            regular_font,
            11.0F,
            box_x + 56.0F,
            box_y + box_size / 2.0F,
            "Ultrasound image placeholder");

  draw_hline(page, left, right, 95.0F);
  draw_text(page, regular_font, 11.0F, left, 78.0F, "Thanks for Reference");
  draw_text(page, regular_font, 11.0F, left + 250.0F, 78.0F, "****End of Report****");

  draw_text(page, bold_font, 11.0F, left, 50.0F, "Radiologic Technologists");
  draw_text(page, bold_font, 11.0F, left + 240.0F, 50.0F, doctor.name);
  draw_text(page, bold_font, 11.0F, left + 450.0F, 50.0F, "Dr. Vimal Shah");

  const HPDF_STATUS save_result = HPDF_SaveToFile(pdf, output_pdf_path.c_str());
  HPDF_Free(pdf);

  return save_result == HPDF_OK;
}

}  // namespace libharu_examples
