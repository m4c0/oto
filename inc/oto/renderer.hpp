#pragma once

#include <chrono>
#include <memory>
#include <span>

namespace oto::r {
  class texture;
  struct deleter {
    void operator()(texture *) const;
  };
}
namespace oto {
  using clock = std::chrono::system_clock;
  using texture = std::unique_ptr<r::texture, r::deleter>;
}
namespace oto::r {
  using audio_callback_t = void (*)(std::span<float>);

  void prepare();
  void present();

  void draw(const oto::texture & txt);

  void set_audio_callback(audio_callback_t cbk);

  [[nodiscard]] oto::texture create_color_texture(int width, int height, int rgb);
}
