#pragma once

#include "oto/renderer.hpp"
#include "oto/scene.hpp"

namespace oto {
  template<typename Tp, typename D>
  concept assets = domain<D> && requires {
    { Tp::load_background(typename D::background()) } -> same_as<oto::texture>;
  };
}
