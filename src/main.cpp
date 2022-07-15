#include "casein.sdl.hpp"
#include "frame_state.hpp"
#include "oto/engine.hpp"

#include <memory>

void casein_sdl_event(const casein::event & evt) {
  static std::unique_ptr<oto::v_engine> eng;

  switch (evt.type()) {
  case casein::CREATE_WINDOW: {
    void * hnd = evt.as<casein::events::create_window>().native_window_handle();
    eng.reset(oto::create_instance(hnd));
    break;
  }
  case casein::REPAINT:
    if (eng) eng->run_frame();
    break;
  default:
    break;
  }
}
