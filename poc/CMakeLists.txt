function(dumper NAME)
  set(TARGET dump-${NAME})

  add_executable(${TARGET})
  target_sources(${TARGET} PRIVATE dump-${NAME}.cpp)
  target_link_libraries(${TARGET} PRIVATE oto-headers)

  add_test(NAME ${TARGET}-create COMMAND ${TARGET})
  add_test(NAME ${TARGET}-compare
    COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
    ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET}.yaml
    ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.yaml)
  set_tests_properties(${TARGET}-compare PROPERTIES DEPENDS ${TARGET}-create)
endfunction()

dumper(action)
dumper(assets)
dumper(yaml)

add_executable(poc MACOSX_BUNDLE WIN32)
target_sources(poc PRIVATE main.cpp)
target_link_libraries(poc PRIVATE oto)

