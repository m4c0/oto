#include "SDL.h"
#include "sdl.hpp"

#include <memory>
#include <stdexcept>

[[nodiscard]] static auto & renderer() {
  static std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> rnd { nullptr, &SDL_DestroyRenderer };
  return rnd;
}

static void init_renderer(void * handle) {
  static std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> wnd { nullptr, &SDL_DestroyWindow };

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    throw std::runtime_error("Failed to initialise SDL");
  }

  wnd.reset(SDL_CreateWindowFrom(handle));
  if (!wnd) {
    throw std::runtime_error("Failed to create SDL window");
  }

  auto & rnd = renderer();
  rnd.reset(SDL_CreateRenderer(wnd.get(), -1, 0));
  if (!rnd) {
    throw std::runtime_error("Failed to create SDL renderer");
  }
}

static void init_audio() {
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
}

void oto::init_sdl(void * handle) {
  init_renderer(handle);
  init_audio();

  static constexpr const auto FULL_BRIGHT = 255;
  auto * rnd = renderer().get();
  SDL_SetRenderDrawColor(rnd, FULL_BRIGHT, 0, FULL_BRIGHT, FULL_BRIGHT);
  SDL_RenderClear(rnd);
  SDL_RenderPresent(rnd);
}
