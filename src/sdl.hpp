#pragma once

struct SDL_Renderer;

namespace oto {
  class sdl {
    SDL_Renderer * renderer;

  public:
    explicit sdl(void * handle);

    void repaint();
  };
}
