#include "oto/engine.hpp"
#include "poc.hpp"

static void music_callback(std::span<float> /**/) {
}

static constexpr const unsigned act_color(poc::domain::actor act) {
  switch (act) {
  case poc::domain::lefty:
    return 0xaa00;
  case poc::domain::midly:
    return 0xaa0000;
  case poc::domain::midly_hot:
    return 0xff0000;
  case poc::domain::righty:
    return 0xaa000000;
  }
}
static constexpr const unsigned bg_color(poc::domain::background bck) {
  switch (bck) {
  case poc::domain::title:
    return 0x7700;
  case poc::domain::restaurant:
    return 0x770000;
  case poc::domain::end:
    return 0x77000000;
  }
}
struct asset {
  static oto::texture load_actor(poc::domain::actor act) {
    return oto::r::create_color_texture(128, 128, act_color(act));
  }
  static oto::texture load_background(poc::domain::background bck) {
    return oto::r::create_color_texture(128, 128, bg_color(bck));
  }
  static auto load_music(poc::domain::music bck) {
    return music_callback;
  }
};

oto::v_engine * oto::create_engine() {
  return new oto::engine<poc::domain, asset>(&poc::game);
}
