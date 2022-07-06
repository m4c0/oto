#pragma once

#include "oto/dumper.exceptions.hpp"
#include "oto/scene.hpp"

#include <iostream>
#include <ostream>

namespace oto::dumper {
  template<template<typename> typename Tool, typename Domain>
  int dump(std::ostream & out, oto::transition<Domain> d) {
    try {
      Tool<Domain>(out).dump(d);
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
}
