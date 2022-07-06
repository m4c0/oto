#include "oto/dumper.yaml.hpp"
#include "poc.hpp"

#include <fstream>
#include <iostream>

namespace poc {}

template<template<typename> typename T>
int tool(const std::string & pname) {
  try {
    std::string fname = pname + ".yaml";
    std::ofstream out { fname, std::ios::trunc };
    if (!out) {
      std::cerr << "ERROR: failed to open file - " << fname << "\n";
      return 1;
    }
    T<poc::domain>(out).dump(poc::game);
  } catch (const oto::dumper::unnamed_choice & e) {
    std::cerr << "ERROR: Found unnamed choice\n";
    return 1;
  } catch (const oto::dumper::unnamed_scene & e) {
    std::cerr << "ERROR: Found unnamed scene\n";
    return 1;
  } catch (const oto::dumper::actor_without_cast & e) {
    std::cerr << "ERROR: Found actor without cast - " << e.what() << "\n";
    return 1;
  }
  return 0;
}
int main(int /*argc*/, char ** argv) {
  return tool<oto::dumper::yaml>(*argv);
}
