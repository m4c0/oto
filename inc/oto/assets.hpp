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
    { Tp::load_text_background(typename D::background()) } -> same_as<oto::texture>;
    { Tp::load_text_font(typename D::background()) } -> same_as<oto::texture>;
    { Tp::rect_of_side(typename D::side()) } -> same_as<rect>;
  };
}
