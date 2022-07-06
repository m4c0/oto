#pragma once

#include "oto/dumper.exceptions.hpp"
#include "oto/scene.hpp"
#include "oto/vm.hpp"

#include <ostream>
#include <variant>

namespace oto::dumper {
  template<typename D>
  class action {
    using tt = transition<D>;

    std::ostream & m_out;
    vm<D> m_vm {};

  public:
    explicit action(std::ostream & o) : m_out(o) {
    }

    virtual const oto::transition<D> * choose(opcodes::choose<D> c) = 0;

    void operator()(std::monostate /**/) {
      m_out << "----\n";
    }
    void operator()(opcodes::background<D> b) {
      m_out << "background " << D::background_to_cstr(*b) << "\n";
    }
    void operator()(opcodes::choose<D> c) {
      m_out << "choose";
      for (auto & cc : c) {
        m_out << " " << cc.name;
      }
      m_out << "\n";
      m_vm = vm<D> { choose(c) };
    }
    void operator()(opcodes::music<D> m) {
      m_out << "music " << D::music_to_cstr(*m) << "\n";
    }
    void operator()(opcodes::pause /*p*/) {
      m_out << "pause\n";
    }
    void operator()(opcodes::present /*p*/) {
      m_out << "present\n";
    }
    void operator()(opcodes::speak<D> m) {
      m_out << "speak " << D::side_to_cstr(m.side) << " " << D::actor_to_cstr(m.actor) << " " << m.text << "\n";
    }

    void dump(tt t) {
      m_vm = vm<D> { &t };
      while (m_vm) {
        auto op = m_vm.iterate();
        std::visit(*this, op);
      }
    }
  };
}
