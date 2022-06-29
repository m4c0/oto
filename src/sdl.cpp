#include "sdl.hpp"

#include <SDL.h>
#include <memory>
#include <stdexcept>

static SDL_Renderer * renderer; // NOLINT

static void init_audio() {
  if (SDL_Init(SDL_INIT_AUDIO) != 0) {
    throw std::runtime_error("Failed to initialise SDL audio");
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
}

void oto::init_sdl(void * handle) {
  renderer = static_cast<SDL_Renderer *>(handle);

  init_audio();
}

void oto::repaint_sdl() {
  static constexpr const auto FULL_BRIGHT = 255;
  const auto r = SDL_GetTicks() % FULL_BRIGHT;
  SDL_SetRenderDrawColor(renderer, r, 0, FULL_BRIGHT, FULL_BRIGHT);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}
