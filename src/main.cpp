#include "casein.hpp"
#include "oto/renderer.hpp"
#include "oto/v_engine.hpp"

#include <memory>

oto::r::audio_callback_t & oto::r::audio_callback() {
  static void (*g_audio_callback)(std::span<float>) = [](auto data) {
    std::fill(data.begin(), data.end(), 0);
  };
  return g_audio_callback;
}

void oto::r::draw_string(const oto::texture & font, const size & chr_size, std::string_view str, int x, int y) {
  rect tgt { .x = x, .y = y, .w = chr_size.w, .h = chr_size.h };
  for (auto chr : str) {
    int cx = static_cast<int>(chr) * chr_size.w;
    rect from { .x = cx, .y = 0, .w = chr_size.w, .h = chr_size.h };
    draw(font, from, tgt);
    tgt.x += chr_size.w;
  }
}

void casein_event(const casein::event & evt) {
  static std::unique_ptr<oto::v_engine> eng;

  switch (evt.type()) {
  case casein::CREATE_WINDOW:
    oto::r::init(evt.as<casein::events::create_window>().native_window_handle());
    eng.reset(oto::create_engine());
    break;
  case casein::KEY_DOWN:
    if (eng) eng->key_down();
    break;
  case casein::REPAINT:
    if (eng) eng->run_frame();
    break;
  default:
    break;
  }
}
