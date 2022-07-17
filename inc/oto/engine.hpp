#pragma once

#include "oto/assets.hpp"
#include "oto/renderer.hpp"
#include "oto/scene.hpp"
#include "oto/v_engine.hpp"
#include "oto/vm.hpp"

namespace oto {
  template<domain D, assets<D> A>
  class engine : public v_engine {
    enum state { run, pause, sleep, speak };

    texture m_actor {};
    texture m_background {};
    std::chrono::time_point<clock> m_timer {};
    state m_state = run;

    vm<D> m_vm;

    state step() {
      switch (m_state) {
      case run:
        return m_vm ? std::visit(*this, m_vm.iterate()) : pause;
      case sleep:
        return (clock::now() > m_timer) ? run : sleep;
      default:
        return m_state;
      }
    }

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
      using namespace std::literals;
      m_timer = clock::now() + 2s;
      return sleep;
    }
    state operator()(const opcodes::speak<D> & spk) {
      m_actor = A::load_actor(spk.actor);
      return speak;
    }
    state operator()(std::monostate /**/) {
      return pause;
    }

    void key_down() override {
      if (m_state == pause) {
        m_state = run;
        m_actor.reset();
      }
      if (m_state == speak) m_state = run;
    }

    void run_frame() override {
      do {
        m_state = step();
      } while (m_state == run);

      oto::r::prepare();
      if (m_background) oto::r::draw(m_background);
      if (m_actor) oto::r::draw(m_actor);
      oto::r::present();
    }
  };
}
