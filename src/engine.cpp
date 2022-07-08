#include "oto/engine.hpp"

#include <SDL.h>
#include <stdexcept>

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

oto::v_engine::v_engine(void * rnd) : m_renderer(static_cast<SDL_Renderer *>(rnd)) {
  init_audio();
}

oto::v_engine::~v_engine() {
  SDL_DestroyTexture(m_background);
}

void oto::v_engine::repaint() {
  static constexpr const auto FULL_BRIGHT = 255;
  SDL_SetRenderDrawColor(m_renderer, FULL_BRIGHT, 0, FULL_BRIGHT, FULL_BRIGHT);
  SDL_RenderClear(m_renderer);

  SDL_RenderCopy(m_renderer, m_background, nullptr, nullptr);

  SDL_RenderPresent(m_renderer);
}
