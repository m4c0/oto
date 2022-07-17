#pragma once

#include "oto/assets.hpp"
#include "oto/renderer.hpp"
#include "oto/scene.hpp"
#include "oto/v_engine.hpp"
#include "oto/vm.hpp"

namespace oto {
  template<domain D, assets<D> A>
  class engine : public v_engine {
    enum state { run, pause };

    texture m_background {};
    int m_timer {};
    state m_state = run;

    vm<D> m_vm;

  public:
    explicit engine(const transition<D> * init) : m_vm(init) {
    }

    state operator()(const opcodes::background<D> & bck) {
      m_background = A::load_background(*bck);
      return run;
    }
    state operator()(const opcodes::choose<D> & /**/) {
      return pause;
    }
    state operator()(const opcodes::music<D> & mus) {
      oto::r::set_audio_callback(A::load_music(*mus));
      return run;
    }
    state operator()(const opcodes::pause & /**/) {
      return pause;
    }
    state operator()(const opcodes::present & /**/) {
      return pause;
    }
    state operator()(const opcodes::speak<D> & /**/) {
      return pause;
    }
    state operator()(std::monostate /**/) {
      return pause;
    }

    void run_frame() override {
      while (m_vm && m_state == run) {
        m_state = std::visit(*this, m_vm.iterate());
      }

      oto::r::prepare();
      if (m_background) oto::r::draw(m_background);
      oto::r::present();
    }
  };
}
