include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(libharu_examples_tests
  test_pdf_text_example.cpp
)

target_link_libraries(libharu_examples_tests
  PRIVATE
    libharu_examples::libharu_examples
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(libharu_examples_tests)
