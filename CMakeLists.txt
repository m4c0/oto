cmake_minimum_required(VERSION 3.23)
project(oto VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()

include(FetchContent)
FetchContent_Declare(
  casein
  GIT_REPOSITORY git@github.com:m4c0/casein
  GIT_TAG 9606de44b9793bbda79dcdebdc62d8f735a783e7)
FetchContent_MakeAvailable(casein)

add_library(oto-headers INTERFACE)
target_compile_features(oto-headers INTERFACE cxx_std_20)
target_include_directories(oto-headers INTERFACE inc)

add_library(oto STATIC)
target_link_libraries(oto PUBLIC oto-headers)

if (${CMAKE_SYSTEM_NAME} STREQUAL "WASI")
  target_sources(oto PRIVATE wasm/main.cpp)
  target_link_libraries(oto PUBLIC casein)
  target_link_options(oto PUBLIC -Wl,--allow-undefined-file=${CMAKE_CURRENT_SOURCE_DIR}/wasm/extern.syms)
else()
  find_package(SDL2 REQUIRED)
  target_sources(oto PRIVATE sdl/main.cpp sdl/renderer.cpp)
  target_link_libraries(oto PUBLIC casein-sdl)
endif()

add_subdirectory(poc)

