#pragma once

#include "oto/scene.hpp"

#include <type_traits>
#include <variant>

namespace oto::opcodes {
  template<typename Arg>
  class one_arg_opcode {
    Arg m_arg;

  public:
    explicit constexpr one_arg_opcode(Arg arg) : m_arg(arg) {
    }

    [[nodiscard]] Arg operator*() const noexcept {
      return m_arg;
    }
  };

  template<domain D>
  using background = one_arg_opcode<typename D::background>;
  template<domain D>
  using choose = choices<D>;
  template<domain D>
  using music = one_arg_opcode<typename D::music>;
  struct pause {};
  struct present {};
  template<domain D>
  struct speak {
    typename D::side side;
    typename D::actor actor;
    const char * text;
  };
}

namespace oto {
  template<domain D>
  using opcode = std::variant<
      std::monostate,
      opcodes::background<D>,
      opcodes::choose<D>,
      opcodes::music<D>,
      opcodes::pause,
      opcodes::present,
      opcodes::speak<D>>;

  template<domain D>
  class vm {
    static_assert(std::is_trivially_copy_assignable<opcode<D>>() && sizeof(opcode<D>) < 32);

    enum state { bg, m, scr, n };

    const transition<D> * m_t = nullptr;
    state m_state = bg;
    int m_n = 0;

  public:
    explicit constexpr vm() = default;

    explicit constexpr vm(const transition<D> * t) : m_t(t) {
    }

    [[nodiscard]] constexpr explicit operator bool() const noexcept {
      return m_t != nullptr;
    }

    [[nodiscard]] constexpr opcode<D> iterate() {
      const auto & s = m_t->from;
      switch (m_state) {
      case bg:
        if (s.background) {
          m_state = m;
          return opcodes::background<D> { *s.background };
        }
      case m:
        if (m_t->from.music) {
          m_state = scr;
          m_n = 0;
          return opcodes::music<D> { *s.music };
        }
      case scr:
        if (m_n < s.script.size()) {
          auto & l = s.script[m_n++];
          return opcodes::speak<D> {
            .side = *s.cast(l.actor),
            .actor = l.actor,
            .text = l.text,
          };
        }
      default:
        m_state = bg;

        switch (m_t->choices.size()) {
        case 0:
          m_t = nullptr;
          return {};
        case 1: {
          auto * front = &m_t->choices.front();
          m_t = &front->next;
          if (front->name == nullptr) {
            return opcodes::present {};
          }
          return opcodes::pause {};
        }
        default:
          auto c = m_t->choices;
          m_t = nullptr;
          return c;
        }
        return {};
      }
    }
  };
}