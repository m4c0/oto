#include "casein.hpp"
#include "oto/v_engine.hpp"
#include "renderer.hpp"

#include <memory>

void casein_event(const casein::event & evt) {
  static std::unique_ptr<oto::v_engine> eng;

  switch (evt.type()) {
  case casein::CREATE_WINDOW:
    oto::r::init(evt.as<casein::events::create_window>().native_window_handle());
    eng.reset(oto::create_engine());
    break;
  case casein::KEY_DOWN:
    if (eng) eng->key_down();
    break;
  case casein::REPAINT:
    if (eng) eng->run_frame();
    break;
  default:
    break;
  }
}
