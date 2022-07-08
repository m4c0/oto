#pragma once

#include "oto/scene.hpp"
#include "oto/vm.hpp"

struct SDL_Renderer;
struct SDL_Texture;

namespace oto {
  class v_engine {
    SDL_Renderer * m_renderer;

  protected:
    explicit v_engine(void * rnd);

    v_engine(v_engine &&) = default;
    v_engine(const v_engine &) = default;
    v_engine & operator=(v_engine &&) = default;
    v_engine & operator=(const v_engine &) = default;

  public:
    virtual ~v_engine() = default;

    void repaint();
  };

  template<domain D>
  class engine : public v_engine {
    SDL_Texture * m_background {};
    vm<D> m_vm {};
    int m_timer {};

  public:
    explicit engine(void * rnd) : v_engine(rnd) {
    }
  };

  v_engine * create_instance(void * rnd);
}
