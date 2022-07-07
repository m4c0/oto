#include "casein.sdl.hpp"
#include "frame_state.hpp"
#include "sdl.hpp"

#include <memory>

void casein_sdl_event(const casein::event & evt) {
  static oto::frame_state frame_state;
  static std::unique_ptr<oto::sdl> sdl;

  switch (evt.type()) {
  case casein::CREATE_WINDOW:
    sdl = std::make_unique<oto::sdl>(evt.as<casein::events::create_window>().native_window_handle());
    break;
  case casein::REPAINT:
    if (sdl) sdl->repaint();
    frame_state = {};
    break;
  default:
    break;
  }
}
