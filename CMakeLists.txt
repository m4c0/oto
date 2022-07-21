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

find_package(SDL2 REQUIRED)

add_library(oto-headers INTERFACE)
target_compile_features(oto-headers INTERFACE cxx_std_20)
target_include_directories(oto-headers INTERFACE inc)

add_library(oto STATIC)
target_sources(oto PRIVATE src/main.cpp src/renderer.cpp)
target_link_libraries(oto PUBLIC casein-sdl oto-headers)

add_subdirectory(poc)

