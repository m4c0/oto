function(dumper NAME)
  add_executable(poc-${NAME})
  target_sources(poc-${NAME} PRIVATE poc-${NAME}.cpp)
  target_link_libraries(poc-${NAME} PRIVATE oto-headers)

  add_test(NAME poc-${NAME}-create COMMAND poc-${NAME})
  add_test(NAME poc-${NAME}-compare
    COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
            ${CMAKE_CURRENT_SOURCE_DIR}/poc-${NAME}.yaml
            ${CMAKE_CURRENT_BINARY_DIR}/poc-${NAME}.yaml)
  set_tests_properties(poc-${NAME}-compare PROPERTIES DEPENDS poc-${NAME}-create)
endfunction()

dumper(action)
dumper(assets)
dumper(yaml)

add_executable(poc MACOSX_BUNDLE WIN32)
target_sources(poc PRIVATE poc.cpp)
target_link_libraries(poc PRIVATE oto)

