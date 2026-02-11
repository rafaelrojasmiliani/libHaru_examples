add_executable(text_example
  text/text_example_main.cpp
)

target_link_libraries(text_example
  PRIVATE
    libharu_examples::libharu_examples
)

add_custom_command(TARGET text_example POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/text
    $<TARGET_FILE_DIR:text_example>/text
)


add_executable(invoice_example
  invoice/invoice_example_main.cpp
)

target_link_libraries(invoice_example
  PRIVATE
    libharu_examples::libharu_examples
)


add_executable(invoice_formal_example
  invoice/invoice_formal_example_main.cpp
)

target_link_libraries(invoice_formal_example
  PRIVATE
    libharu_examples::libharu_examples
)


add_executable(clinical_report_example
  clinical/clinical_report_example_main.cpp
)

target_link_libraries(clinical_report_example
  PRIVATE
    libharu_examples::libharu_examples
)
