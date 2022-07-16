#include "casein.sdl.hpp"
#include "frame_state.hpp"
#include "oto/v_engine.hpp"
#include "renderer.hpp"

#include <memory>

void casein_sdl_event(const casein::event & evt) {
  static std::unique_ptr<oto::v_engine> eng;
  static std::unique_ptr<oto::sdl_renderer> rnd;

  switch (evt.type()) {
  case casein::CREATE_WINDOW: {
    void * hnd = evt.as<casein::events::create_window>().native_window_handle();
    rnd = std::make_unique<oto::sdl_renderer>(hnd);
    eng.reset(oto::create_engine(rnd.get()));
    break;
  }
  case casein::REPAINT:
    if (eng) eng->run_frame();
    break;
  default:
    break;
  }
}
