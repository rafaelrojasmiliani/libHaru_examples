#pragma once

#include <string>

namespace libharu_examples {

class ClinicalReportExample {
 public:
  struct Patient {
    std::string full_name;
    int age;
    std::string sex;
    std::string patient_id;
  };

  struct ReferringDoctor {
    std::string name;
    std::string specialty;
  };

  bool create_clinical_report_pdf(const Patient& patient,
                                  const ReferringDoctor& doctor,
                                  const std::string& output_pdf_path) const;
};

}  // namespace libharu_examples
