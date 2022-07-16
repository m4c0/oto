#pragma once

#include <memory>

namespace oto::r {
  class texture;
  struct deleter {
    void operator()(texture *) const;
  };
}
namespace oto {
  using texture = std::unique_ptr<r::texture, r::deleter>;
}
namespace oto::r {
  void prepare();
  void present();

  void draw(const oto::texture & txt);

  [[nodiscard]] oto::texture create_color_texture(int width, int height, int rgb);
}
