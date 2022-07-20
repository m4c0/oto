#pragma once

#include <chrono>
#include <memory>
#include <span>
#include <string_view>

namespace oto::r {
  class texture_ptr;
  struct deleter {
    void operator()(texture_ptr *) const;
  };
}
namespace oto {
  using clock = std::chrono::system_clock;
  using texture = std::unique_ptr<r::texture_ptr, r::deleter>;

  struct size {
    int w, h;
  };
  struct rect {
    int x, y;
    int w, h;
  };
}
namespace oto::r {
  using audio_callback_t = void (*)(std::span<float>);

  void draw(const oto::texture & txt);
  void draw(const oto::texture & txt, const rect & rect);
  void draw(const oto::texture & txt, const rect & clip, const rect & target);
  void draw_string(const oto::texture & font, const size & chr_size, std::string_view str, int x, int y);

  void set_audio_callback(audio_callback_t cbk);

  [[nodiscard]] oto::texture create_color_texture(int width, int height, unsigned rgb);
  [[nodiscard]] oto::texture create_rgba_texture(int width, int height, std::span<const unsigned> data);
}
