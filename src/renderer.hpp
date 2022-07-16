#pragma once

#include "oto/renderer.hpp"

struct SDL_Renderer;

namespace oto {
  class sdl_renderer : public renderer {
    SDL_Renderer * m_renderer;

  public:
    explicit sdl_renderer(void * rnd);

    void prepare() override;
    void present() override;

    void draw(const texture & txt) override;

    [[nodiscard]] texture create_color_texture(int width, int height, int rgb) const override;
  };
}
