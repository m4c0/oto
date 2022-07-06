#pragma once

#include "oto/scene.hpp"

#include <variant>

namespace oto::opcodes {
  template<domain D, typename Arg>
  class one_arg_opcode {
    Arg m_arg;

  public:
    explicit constexpr one_arg_opcode(Arg a) : m_arg(a) {
    }

    [[nodiscard]] Arg operator*() const noexcept {
      return m_arg;
    }
  };
  template<domain D>
  struct line {
    typename D::side side;
    typename D::actor actor;
    const char * text;
  };

  template<domain D>
  using background = one_arg_opcode<D, typename D::background>;
  template<domain D>
  using choose = one_arg_opcode<D, choices<D>>;
  template<domain D>
  using music = one_arg_opcode<D, typename D::music>;
  struct pause {};
  struct present {};
  template<domain D>
  using speak = one_arg_opcode<D, line<D>>;
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
  public:
    explicit vm(const transition<D> * t) {
    }

    [[nodiscard]] constexpr opcode<D> iterate() const {
      return {};
    }
  };
}
