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

class poc_engine : public oto::engine<poc::domain> {
protected:
  SDL_Surface * load_background(poc::domain::background bck) override {
    return create_color_surface(128, 128, bg_color(bck));
  }

public:
  explicit poc_engine(void * rnd) : engine(rnd, &poc::game) {
  }
};

oto::v_engine * oto::create_instance(void * rnd) {
  return new poc_engine(rnd);
}
