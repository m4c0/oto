#include "casein.sdl.hpp"
#include "sdl.hpp"

void casein_sdl_event(const casein::event & e) {
  switch (e.type()) {
  case casein::CREATE_WINDOW:
    oto::init_sdl(e.as<casein::events::create_window>().native_window_handle());
    break;
  case casein::REPAINT:
    oto::repaint_sdl();
    break;
  default:
    break;
  }
}
