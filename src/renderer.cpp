#include "renderer.hpp"

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

oto::sdl_renderer::sdl_renderer(void * rnd) : m_renderer(static_cast<SDL_Renderer *>(rnd)) {
  init_audio();
}

void oto::sdl_renderer::prepare() {
  static constexpr const auto FULL_BRIGHT = 255;
  SDL_SetRenderDrawColor(m_renderer, FULL_BRIGHT, 0, FULL_BRIGHT, FULL_BRIGHT);
  SDL_RenderClear(m_renderer);
}

void oto::sdl_renderer::present() {
  SDL_RenderPresent(m_renderer);
}

oto::texture oto::sdl_renderer::create_color_texture(int width, int height, int rgb) const {
  static constexpr const auto bits_per_pixel = 24;
  static constexpr const auto pixel_format = SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGB888;
  SDL_Surface * surf = SDL_CreateRGBSurfaceWithFormat(0, width, height, bits_per_pixel, pixel_format);

  SDL_Rect rect { 0, 0, width, height };
  SDL_FillRect(surf, &rect, rgb);

  SDL_Texture * txt = SDL_CreateTextureFromSurface(m_renderer, surf);
  SDL_FreeSurface(surf);

  return oto::texture(reinterpret_cast<texture_ptr *>(txt));
}

void oto::sdl_renderer::draw(const texture & txt) {
  SDL_RenderCopy(m_renderer, reinterpret_cast<SDL_Texture *>(txt.get()), nullptr, nullptr);
}

void oto::r_deleter::operator()(texture_ptr * txt) const {
  SDL_DestroyTexture(reinterpret_cast<SDL_Texture *>(txt));
}
