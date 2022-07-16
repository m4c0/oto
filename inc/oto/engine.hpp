#pragma once

#include "oto/renderer.hpp"
#include "oto/scene.hpp"
#include "oto/v_engine.hpp"
#include "oto/vm.hpp"

namespace oto {
  template<domain D>
  class engine : public v_engine {
    renderer * m_renderer;
    texture m_background {};
    int m_timer {};

    vm<D> m_vm;

  protected:
    [[nodiscard]] constexpr auto * renderer() const noexcept {
      return m_renderer;
    }

    virtual texture load_background(typename D::background bck) = 0;

  public:
    explicit engine(struct renderer * rnd, const transition<D> * init) : m_renderer(rnd), m_vm(init) {
    }

    void operator()(const opcodes::background<D> & bck) {
      m_background = load_background(*bck);
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

      m_renderer->prepare();
      if (m_background) m_renderer->draw(m_background);
      m_renderer->present();
    }
  };
}
