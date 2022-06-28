#include "casein.hpp"
#include "sdl.hpp"

static void on_window_created(const casein::event & e) {
  oto::init_sdl(e.as<casein::events::create_window>().native_window_handle());
}

void casein_event(const casein::event & e) {
  switch (e.type()) {
  case casein::CREATE_WINDOW:
    on_window_created(e);
    break;
  default:
    break;
  }
}
