cmake_minimum_required(VERSION 3.20)
project(libHaru_examples VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTING "Build unit tests" ON)
option(LIBHARU_EXAMPLES_AUTO_INIT_SUBMODULES
  "Automatically initialize git submodules during configure"
  ON
)

find_package(Git QUIET)
if(LIBHARU_EXAMPLES_AUTO_INIT_SUBMODULES
   AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git"
   AND GIT_FOUND)
  execute_process(
    COMMAND "${GIT_EXECUTABLE}" submodule update --init --recursive -- libharu
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    RESULT_VARIABLE LIBHARU_SUBMODULE_RESULT
    OUTPUT_QUIET
    ERROR_QUIET
  )
  if(NOT LIBHARU_SUBMODULE_RESULT EQUAL 0)
    message(WARNING "Automatic submodule initialization failed; falling back to local checkout state.")
  endif()
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libharu/CMakeLists.txt")
  message(FATAL_ERROR
    "The libharu submodule is not available. Run:\n"
    "  git submodule update --init --recursive\n"
    "Or configure with -DLIBHARU_EXAMPLES_AUTO_INIT_SUBMODULES=ON from a git checkout.")
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
