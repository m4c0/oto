#include "SDL.h"
#include "sdl.hpp"

#include <memory>
#include <stdexcept>

void oto::init_sdl(void * handle) {
  static std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> wnd { nullptr, &SDL_DestroyWindow };
  static std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> rnd { nullptr, &SDL_DestroyRenderer };

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    throw std::runtime_error("Failed to initialise SDL");
  }

  wnd.reset(SDL_CreateWindowFrom(handle));
  if (!wnd) {
    throw std::runtime_error("Failed to create SDL window");
  }

  rnd.reset(SDL_CreateRenderer(wnd.get(), -1, 0));
  if (!rnd) {
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

  SDL_RenderClear(rnd.get());
  SDL_RenderPresent(rnd.get());
}
