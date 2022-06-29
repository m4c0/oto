#include "casein.sdl.hpp"
#include "sdl.hpp"

#include <memory>

void casein_sdl_event(const casein::event & e) {
  static std::unique_ptr<oto::sdl> sdl;

  switch (e.type()) {
  case casein::CREATE_WINDOW:
    sdl = std::make_unique<oto::sdl>(e.as<casein::events::create_window>().native_window_handle());
    break;
  case casein::REPAINT:
    if (sdl) sdl->repaint();
    break;
  default:
    break;
  }
}
