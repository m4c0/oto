cmake_minimum_required(VERSION 3.19)
project(pegasus_cpp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

find_program(OCAML ocamlc REQUIRED)
execute_process(
  COMMAND ${OCAML} -where
  OUTPUT_VARIABLE OCAML_INC
  OUTPUT_STRIP_TRAILING_WHITESPACE
  COMMAND_ERROR_IS_FATAL ANY
  )

add_library(pegasus_cpp STATIC main.cpp)
target_include_directories(pegasus_cpp PRIVATE ${OCAML_INC})
target_link_libraries(pegasus_cpp PRIVATE SDL2::SDL2-static)
