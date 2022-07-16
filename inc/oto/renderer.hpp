#pragma once

#include <memory>

namespace oto {
  class texture_ptr;
  struct r_deleter {
    void operator()(texture_ptr *) const;
  };
  using texture = std::unique_ptr<texture_ptr, r_deleter>;

  struct renderer {
    virtual ~renderer() = default;

    virtual void prepare() = 0;
    virtual void present() = 0;

    virtual void draw(const texture & txt) = 0;

    [[nodiscard]] virtual texture create_color_texture(int width, int height, int rgb) const = 0;
  };
}
