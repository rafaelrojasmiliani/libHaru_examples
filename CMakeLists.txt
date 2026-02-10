cmake_minimum_required(VERSION 3.20)
project(libHaru_examples VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTING "Build unit tests" ON)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libharu/CMakeLists.txt")
  message(FATAL_ERROR
    "The libharu submodule is not available. Run:\n"
    "  git submodule update --init --recursive")
endif()

add_subdirectory(libharu)

add_library(libharu_examples
  src/pdf_text_example.cpp
)

add_library(libharu_examples::libharu_examples ALIAS libharu_examples)

target_include_directories(libharu_examples
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libharu/include
)

target_link_libraries(libharu_examples
  PUBLIC
    hpdf
)

add_subdirectory(examples)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
