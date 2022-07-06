#pragma once

#include <exception>

namespace oto::dumper {
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
}
