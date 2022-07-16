#pragma once

#include "oto/renderer.hpp"
#include "oto/scene.hpp"
#include "oto/v_engine.hpp"
#include "oto/vm.hpp"

namespace oto {
  template<domain D>
  class engine : public v_engine {
    texture m_background {};
    int m_timer {};

    vm<D> m_vm;

  protected:
    virtual texture load_background(typename D::background bck) = 0;

  public:
    explicit engine(const transition<D> * init) : m_vm(init) {
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

      oto::r::prepare();
      if (m_background) oto::r::draw(m_background);
      oto::r::present();
    }
  };
}
