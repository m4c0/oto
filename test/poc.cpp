#include "oto/engine.hpp"
#include "poc.hpp"

oto::v_engine * oto::create_instance(void * rnd) {
  return new oto::engine<poc::domain>(rnd);
}
