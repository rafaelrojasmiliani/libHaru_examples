#include "libharu_examples/clinical_report_example.h"

#include <iostream>

int main(int argc, char** argv) {
  const std::string output_pdf = (argc > 1) ? argv[1] : "clinical_report_example.pdf";

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

  libharu_examples::ClinicalReportExample example;
  if (!example.create_clinical_report_pdf(patient, doctor, output_pdf)) {
    std::cerr << "Failed to create clinical report PDF: " << output_pdf << '\n';
    return 1;
  }

  std::cout << "Created clinical report PDF: " << output_pdf << '\n';
  return 0;
}
