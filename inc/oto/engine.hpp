#pragma once

#include "oto/scene.hpp"
#include "oto/vm.hpp"

struct SDL_Renderer;
struct SDL_Texture;

namespace oto {
  class v_engine {
    SDL_Renderer * m_renderer;
    SDL_Texture * m_background {};
    int m_timer {};

  protected:
    explicit v_engine(void * rnd);

    v_engine(v_engine &&) = default;
    v_engine(const v_engine &) = default;
    v_engine & operator=(v_engine &&) = default;
    v_engine & operator=(const v_engine &) = default;

  public:
    virtual ~v_engine();

    void repaint();
  };

  template<domain D>
  class engine : public v_engine {
    vm<D> m_vm {};

  public:
    explicit engine(void * rnd) : v_engine(rnd) {
    }
  };

  v_engine * create_instance(void * rnd);
}
