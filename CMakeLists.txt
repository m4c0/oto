cmake_minimum_required(VERSION 3.23)
project(oto VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()

include(FetchContent)
FetchContent_Declare(
  casein
  GIT_REPOSITORY git@github.com:m4c0/casein
  GIT_TAG e32fa538971966f3f9bcab9ea19b434e48a9d649)
FetchContent_MakeAvailable(casein)

find_package(SDL2 REQUIRED)

add_library(oto-headers INTERFACE)
target_compile_features(oto-headers INTERFACE cxx_std_20)
target_include_directories(oto-headers INTERFACE inc)

add_library(oto STATIC)
target_sources(oto PRIVATE src/engine.cpp src/main.cpp)
target_link_libraries(oto PUBLIC casein-sdl oto-headers)

add_executable(poc MACOSX_BUNDLE WIN32)
target_sources(poc PRIVATE test/poc.cpp)
target_link_libraries(poc PRIVATE oto)

function(dumper NAME)
  add_executable(poc-${NAME})
  target_sources(poc-${NAME} PRIVATE test/poc-${NAME}.cpp)
  target_link_libraries(poc-${NAME} PRIVATE oto-headers)

  add_test(NAME poc-${NAME}-create COMMAND poc-${NAME})
  add_test(NAME poc-${NAME}-compare COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol ${CMAKE_CURRENT_SOURCE_DIR}/test/poc-${NAME}.yaml poc-${NAME}.yaml)
  set_tests_properties(poc-${NAME}-compare PROPERTIES DEPENDS poc-${NAME}-create)
endfunction()

dumper(action)
dumper(assets)
dumper(yaml)
