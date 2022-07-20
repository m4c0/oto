#include "assets.hpp"
#include "main.hpp"
#include "oto/engine.hpp"

oto::v_engine * oto::create_engine() {
  return new oto::engine<poc::domain, asset>(&poc::game);
}
