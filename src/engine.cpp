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

void oto::v_engine::paint() {
  static constexpr const auto FULL_BRIGHT = 255;
  SDL_SetRenderDrawColor(m_renderer, FULL_BRIGHT, 0, FULL_BRIGHT, FULL_BRIGHT);
  SDL_RenderClear(m_renderer);

  SDL_RenderCopy(m_renderer, m_background, nullptr, nullptr);

  SDL_RenderPresent(m_renderer);
}

void oto::v_engine::set_background(SDL_Surface * bkg) {
  SDL_DestroyTexture(m_background);
  m_background = SDL_CreateTextureFromSurface(m_renderer, bkg);
  SDL_FreeSurface(bkg);
}

SDL_Surface * oto::v_engine::create_color_surface(int width, int height, int rgb) const {
  static constexpr const auto bits_per_pixel = 24;
  static constexpr const auto pixel_format = SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGB888;
  SDL_Surface * surf = SDL_CreateRGBSurfaceWithFormat(0, width, height, bits_per_pixel, pixel_format);

  SDL_Rect rect { 0, 0, width, height };
  SDL_FillRect(surf, &rect, rgb);
  return surf;
}
