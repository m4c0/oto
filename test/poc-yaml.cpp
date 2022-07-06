#include "poc.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <set>

namespace oto {
  class unnamed_choice : public std::exception {};
  class unnamed_scene : public std::exception {};
  class actor_without_cast : public std::exception {
    const char * who;

  public:
    explicit actor_without_cast(const char * w) : who(w) {
    }

    [[nodiscard]] const char * what() const noexcept override {
      return who;
    }
  };

  template<typename D>
  class dumper {
    using ee = oto::edge<D>;
    using ss = oto::scene_meta<D>;
    using tt = oto::transition<D>;

    std::ofstream out { "poc.yaml" };
    std::set<std::string> done {};

    void dump_actors(ss s) {
      std::set<typename D::actor> names {};
      for (auto l : s.script) {
        names.insert(l.actor);
      }

      out << "actors:\n";
      for (auto a : names) {
        auto an = D::actor_to_cstr(a);

        auto os = s.cast(a);
        if (!os) throw actor_without_cast { an };

        out << "  " << an << ": " << D::side_to_cstr(*os) << "\n";
      }
    }

    void dump_script(ss s) {
      out << "script:\n";
      for (auto l : s.script) {
        auto an = D::actor_to_cstr(l.actor);

        auto os = s.cast(l.actor);
        if (!os) throw actor_without_cast { an };

        out << "  - [" << an << "] " << l.text << "\n";
      }
    }

    void dump_single_transition(ee e) {
      if (e.next.from.name == nullptr) {
        throw unnamed_scene();
      }

      if (e.name == nullptr) {
        out << " jump to ";
      } else {
        out << " pause then move to ";
      }
      out << e.next.from.name << "\n";
    }

    void dump_choices(tt t) {
      out << "\n";
      for (auto e : t.choices) {
        if (e.name == nullptr) {
          throw unnamed_choice();
        }
        if (e.next.from.name == nullptr) {
          throw unnamed_scene();
        }
        out << "  - \"" << e.name << "\" jumps to " << e.next.from.name << "\n";
      }
    }

    void dump_outcome(tt t) {
      out << "outcome:";
      switch (t.choices.size()) {
      case 0:
        out << " end game\n";
        break;
      case 1:
        dump_single_transition(t.choices.front());
        break;
      default:
        dump_choices(t);
        break;
      }
    }

    void dump_scene(tt t) {
      auto s = t.from;

      if (s.name == nullptr) {
        throw unnamed_scene();
      }
      if (done.contains(s.name)) {
        return;
      }

      out << "name: " << s.name << "\n";

      if (s.background) {
        out << "background: " << D::background_to_cstr(*s.background) << "\n";
      }
      if (s.music) {
        out << "music: " << D::music_to_cstr(*s.music) << "\n";
      }

      dump_actors(s);
      dump_script(s);
      dump_outcome(t);

      out << "---\n";
      done.insert(s.name);
    }

  public:
    void dump(tt t) {
      dump_scene(t);
      for (auto c : t.choices) {
        dump(c.next);
      }
    }
  };
}

namespace poc {}

int main() {
  try {
    oto::dumper<poc::domain>().dump(poc::game);
  } catch (const oto::unnamed_choice & e) {
    std::cerr << "ERROR: Found unnamed choice\n";
    return 1;
  } catch (const oto::unnamed_scene & e) {
    std::cerr << "ERROR: Found unnamed scene\n";
    return 1;
  } catch (const oto::actor_without_cast & e) {
    std::cerr << "ERROR: Found actor without cast - " << e.what() << "\n";
    return 1;
  }
}
