#pragma once

#include "oto/scene.hpp"
#include "oto/vm.hpp"

struct SDL_Renderer;
struct SDL_Surface;
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

    void set_background(SDL_Surface * bkg);

    SDL_Surface * create_color_surface(int width, int height, int rgb) const;

    void paint();

  public:
    virtual ~v_engine() = default;

    virtual void run_frame() = 0;
  };

  template<domain D>
  class engine : public v_engine {
    vm<D> m_vm;

  protected:
    virtual SDL_Surface * load_background(typename D::background bck) = 0;

  public:
    explicit engine(void * rnd, const transition<D> * init) : v_engine(rnd), m_vm(init) {
    }

    void operator()(const opcodes::background<D> & bck) {
      set_background(load_background(*bck));
    }
    void operator()(const opcodes::choose<D> & /**/) {
    }
    void operator()(const opcodes::music<D> & /**/) {
    }
    void operator()(const opcodes::pause & /**/) {
    }
    void operator()(const opcodes::present & /**/) {
    }
    void operator()(const opcodes::speak<D> & /**/) {
    }
    void operator()(std::monostate /**/) {
    }

    void run_frame() override {
      if (m_vm) std::visit(*this, m_vm.iterate());
      paint();
    }
  };

  v_engine * create_instance(void * rnd);
}
