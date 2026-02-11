include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)

set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(libharu_examples_tests
  test_pdf_text_example.cpp
  test_invoice_example.cpp
  test_clinical_report_example.cpp
)
target_include_directories(
  libharu_examples_tests
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../libharu/include)

target_link_libraries(libharu_examples_tests PRIVATE hpdf GTest::gtest_main
                                                     libharu_examples)

include(GoogleTest)
gtest_discover_tests(libharu_examples_tests)
