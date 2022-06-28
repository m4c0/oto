#include "SDL.h"
#include "casein.hpp"

#include <stdexcept>

static void on_window_created(const casein::event & e) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    throw std::runtime_error("Failed to initialise SDL");
  }

  void * h = e.as<casein::events::create_window>().native_window_handle();
  SDL_Window * wnd = SDL_CreateWindowFrom(h);
  if (wnd == nullptr) {
    throw std::runtime_error("Failed to create SDL window");
  }

  SDL_Renderer * r = SDL_CreateRenderer(wnd, -1, 0);
  if (r == nullptr) {
    throw std::runtime_error("Failed to create SDL renderer");
  }

  static constexpr const SDL_AudioSpec desired {
    .freq = 44100,
    .format = AUDIO_F32,
    .channels = 1,
    .samples = 4096,
    //.callback = audio_callback,
  };
  SDL_AudioSpec obtained;
  auto dev = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, 0);

  SDL_PauseAudioDevice(dev, 0);

  SDL_RenderClear(r);
  SDL_RenderPresent(r);
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
