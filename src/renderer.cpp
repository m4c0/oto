#include "oto/renderer.hpp"
#include "renderer.hpp"

#include <SDL.h>
#include <span>
#include <stdexcept>

SDL_Renderer * g_renderer = nullptr;
void (*g_audio_callback)(std::span<float>) = [](auto data) {
  std::fill(data.begin(), data.end(), 0);
};

static void audio_callback(void * /*user*/, Uint8 * str, int len) {
  float * fptr = reinterpret_cast<float *>(str);
  g_audio_callback(std::span(fptr, len / sizeof(float)));
}

static void init_audio() {
  if (SDL_Init(SDL_INIT_AUDIO) != 0) {
    throw std::runtime_error("Failed to initialise SDL audio");
  }

  static constexpr const SDL_AudioSpec desired {
    .freq = 44100,
    .format = AUDIO_F32,
    .channels = 1,
    .samples = 4096,
    .callback = audio_callback,
  };
  SDL_AudioSpec obtained;
  auto dev = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, 0);

  SDL_PauseAudioDevice(dev, 0);
}

void oto::r::init(void * hnd) {
  g_renderer = static_cast<SDL_Renderer *>(hnd);

  init_audio();
}

void oto::r::prepare() {
  static constexpr const auto FULL_BRIGHT = 255;

  SDL_SetRenderDrawColor(g_renderer, FULL_BRIGHT, 0, FULL_BRIGHT, FULL_BRIGHT);
  SDL_RenderClear(g_renderer);
}

void oto::r::present() {
  SDL_RenderPresent(g_renderer);
}

oto::texture oto::r::create_color_texture(int width, int height, int rgb) {
  static constexpr const auto bits_per_pixel = 24;
  static constexpr const auto pixel_format = SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGB888;
  SDL_Surface * surf = SDL_CreateRGBSurfaceWithFormat(0, width, height, bits_per_pixel, pixel_format);

  SDL_Rect rect { 0, 0, width, height };
  SDL_FillRect(surf, &rect, rgb);

  SDL_Texture * txt = SDL_CreateTextureFromSurface(g_renderer, surf);
  SDL_FreeSurface(surf);

  return oto::texture(reinterpret_cast<oto::r::texture *>(txt));
}

void oto::r::draw(const oto::texture & txt) {
  SDL_RenderCopy(g_renderer, reinterpret_cast<SDL_Texture *>(txt.get()), nullptr, nullptr);
}

void oto::r::set_audio_callback(audio_callback_t cbk) {
  g_audio_callback = cbk;
}

void oto::r::deleter::operator()(oto::r::texture * txt) const {
  SDL_DestroyTexture(reinterpret_cast<SDL_Texture *>(txt));
}
