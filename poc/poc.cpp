#include "oto/engine.hpp"
#include "poc.assets.hpp"
#include "poc.hpp"

oto::v_engine * oto::create_engine() {
  return new oto::engine<poc::domain, asset>(&poc::game);
}
