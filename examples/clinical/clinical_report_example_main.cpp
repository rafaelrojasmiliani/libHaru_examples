/*
High-level overview
-------------------
This executable demonstrates generation of a clinical ultrasound-style report with
an empty square placeholder reserved for ultrasound image data.

1) Build strongly-typed patient/referring-doctor input records.
2) Call `ClinicalReportExample::create_clinical_report_pdf(...)`.
3) Print a clear success/failure message for CLI users.

libHaru logic addressed by this executable
------------------------------------------
- This layer intentionally avoids low-level libHaru drawing calls.
- It showcases a clean entrypoint pattern where the renderer owns document lifecycle and layout.
*/
#include "libharu_examples/clinical_report_example.h"

#include <iostream>

int main(int argc, char** argv) {
  // Step 1: Resolve output path from command line or default value.
  const std::string output_pdf = (argc > 1) ? argv[1] : "clinical_report_example.pdf";

  // Step 2: Build example clinical metadata used by the report renderer.
  const libharu_examples::ClinicalReportExample::Patient patient{
      "Yashvi M. Patel",
      21,
      "Female",
      "555",
  };

  const libharu_examples::ClinicalReportExample::ReferringDoctor doctor{
      "Dr. Hiren Shah",
      "Radiologist",
  };

  // Step 3: Generate the PDF and map the result to process status.
  libharu_examples::ClinicalReportExample example;
  if (!example.create_clinical_report_pdf(patient, doctor, output_pdf)) {
    std::cerr << "Failed to create clinical report PDF: " << output_pdf << '\n';
    return 1;
  }

  std::cout << "Created clinical report PDF: " << output_pdf << '\n';
  return 0;
}
