#pragma once

#include "oto/renderer.hpp"
#include "oto/scene.hpp"

#include <span>

namespace oto {
  template<typename Tp, typename D>
  concept assets = domain<D> && requires {
    { Tp::load_actor(typename D::actor()) } -> same_as<oto::texture>;
    { Tp::load_background(typename D::background()) } -> same_as<oto::texture>;
    { Tp::load_music(typename D::music()) } -> same_as<r::audio_callback_t>;
  };
}
