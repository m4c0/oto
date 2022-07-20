#pragma once

#include "oto/assets.hpp"
#include "oto/renderer.hpp"
#include "oto/scene.hpp"
#include "oto/v_engine.hpp"
#include "oto/vm.hpp"

#include <string_view>

namespace oto {
  template<domain D, assets<D> A>
  class engine : public v_engine {
    enum state { run, pause, sleep, speak };

    texture m_actor {};
    std::string_view m_actor_name {};
    rect m_actor_rect {};
    texture m_background {};
    state m_state = run;
    texture m_text_background {};
    texture m_text_font {};
    std::string_view m_text_line {};
    size m_text_chr_size {};
    std::chrono::time_point<clock> m_timer {};

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
      m_text_background = A::load_text_background(*bck);
      m_text_font = A::load_text_font(*bck);
      m_text_chr_size = A::size_of_font(*bck);
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
      m_actor_name = A::actor_name(spk.actor);
      m_actor_rect = A::rect_of_side(spk.side);
      m_text_line = spk.text;
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
      static constexpr const rect text_bg_rect = { .x = 0, .y = 400, .w = 800, .h = 200 };
      static constexpr const auto actor_name_x = 10;
      static constexpr const auto actor_name_y = 410;
      static constexpr const auto text_x = 30;
      static constexpr const auto text_y = 480;

      do {
        m_state = step();
      } while (m_state == run);

      if (m_background) oto::r::draw(m_background);
      if (m_actor) oto::r::draw(m_actor, m_actor_rect);

      if (m_state == speak) {
        r::draw(m_text_background, text_bg_rect);
        r::draw_string(m_text_font, m_text_chr_size, m_actor_name, actor_name_x, actor_name_y);
        r::draw_string(m_text_font, m_text_chr_size, m_text_line, text_x, text_y);
      }
    }
  };
}
