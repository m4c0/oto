#include "oto/engine.hpp"
#include "poc.hpp"

static constexpr const auto bg_color(poc::domain::background bck) {
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
  static oto::texture load_background(poc::domain::background bck) {
    return oto::r::create_color_texture(128, 128, bg_color(bck));
  }
};

oto::v_engine * oto::create_engine() {
  return new oto::engine<poc::domain, asset>(&poc::game);
}
