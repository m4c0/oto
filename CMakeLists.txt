cmake_minimum_required(VERSION 3.23)
project(oto VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  casein
  GIT_REPOSITORY git@github.com:m4c0/casein
  GIT_TAG 5d405cd9b0e7afba1391a45221de979466df5d57)
FetchContent_MakeAvailable(casein)

find_package(SDL2 REQUIRED)

add_executable(oto MACOSX_BUNDLE WIN32)
target_sources(oto PRIVATE src/main.cpp)
target_link_libraries(oto PUBLIC casein SDL2::SDL2-static)
