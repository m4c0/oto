cmake_minimum_required(VERSION 3.23)
project(oto VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  casein
  GIT_REPOSITORY git@github.com:m4c0/casein
  GIT_TAG 2cc36b83d7a9b0b355778f892eaa078811fedf62)
FetchContent_MakeAvailable(casein)

find_package(SDL2 REQUIRED)

add_library(oto STATIC)
target_sources(oto PRIVATE src/main.cpp src/sdl.cpp)
target_link_libraries(oto PUBLIC casein-sdl)

add_executable(poc MACOSX_BUNDLE WIN32)
target_sources(poc PRIVATE test/poc.cpp)
target_link_libraries(poc PRIVATE oto)
