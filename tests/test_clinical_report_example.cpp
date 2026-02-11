#include "libharu_examples/clinical_report_example.h"

#include <gtest/gtest.h>

TEST(ClinicalReportExampleTest, ReturnsFalseForInvalidArguments) {
  libharu_examples::ClinicalReportExample example;

  const libharu_examples::ClinicalReportExample::Patient patient{
      "Patient",
      21,
      "Female",
      "123",
  };
  const libharu_examples::ClinicalReportExample::ReferringDoctor doctor{
      "Doctor",
      "Radiology",
  };

  EXPECT_FALSE(example.create_clinical_report_pdf(patient, doctor, ""));
  EXPECT_FALSE(example.create_clinical_report_pdf({}, doctor, "report.pdf"));
  EXPECT_FALSE(example.create_clinical_report_pdf(patient, {}, "report.pdf"));
}
