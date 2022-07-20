#pragma once

#include "main.hpp"
#include "oto/dumper.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace poc {
  template<template<typename> typename T>
  int tool(const std::string & pname) {
    std::filesystem::path fname { pname };
    fname.replace_extension("yaml");

    std::ofstream out { fname, std::ios::trunc };
    if (!out) {
      std::cerr << "ERROR: failed to open file - " << fname << "\n";
      return 1;
    }
    return oto::dumper::dump<T>(out, poc::game);
  }
}
