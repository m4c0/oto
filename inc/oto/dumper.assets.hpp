#pragma once

#include "oto/dumper.exceptions.hpp"
#include "oto/scene.hpp"

#include <ostream>
#include <set>

namespace oto::dumper {
  template<typename D>
  class assets {
    using tt = transition<D>;

    std::ostream & out;
    std::set<std::string> ass {};

    void rec_dump(tt t) {
      auto s = t.from;

      if (s.background) {
        ass.insert(std::string { "back/" } + D::background_to_cstr(*s.background));
      }
      if (s.music) {
        ass.insert(std::string { "music/" } + D::music_to_cstr(*s.music));
      }

      for (auto l : s.script) {
        ass.insert(std::string { "char/" } + D::actor_to_cstr(l.actor));
      }

      for (auto c : t.choices) {
        rec_dump(c.next);
      }
    }

  public:
    explicit assets(std::ostream & o) : out(o) {
    }

    void dump(tt t) {
      rec_dump(t);
      for (const auto & s : ass) {
        out << s << "\n";
      }
    }
  };
}
