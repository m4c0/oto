#pragma once

#include "oto/dumper.exceptions.hpp"
#include "oto/scene.hpp"
#include "oto/vm.hpp"

#include <ostream>

namespace oto::dumper {
  template<typename D>
  class action {
    using tt = transition<D>;

    std::ostream & out;

  public:
    explicit action(std::ostream & o) : out(o) {
    }

    void dump(tt t) {
      vm<D> vm { &t };
      auto i = vm.iterate();
    }
  };
}
