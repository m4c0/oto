#pragma once

#include "poc.scenes.hpp"

namespace oto {
  template<domain D>
  struct transition;

  template<domain D>
  struct edge {
    const char * name;
    transition<D> next;
  };

  template<domain D>
  struct transition {
    scene_meta<D> from;
    std::span<const edge<D>> choices;
  };
}

namespace poc {
  using edg = oto::edge<domain>;
  using trn = oto::transition<domain>;

  static constexpr const trn the_end = { .from = the_end_meta };
  static constexpr const auto end = std::array { edg { "", the_end } };

  static constexpr const trn lefty = { .from = lefty_meta, .choices = end };
  static constexpr const trn midly = { .from = midly_meta, .choices = end };
  static constexpr const trn righty = { .from = righty_meta, .choices = end };

  static constexpr const auto intro_choices = std::array {
    edg { "Lefty", lefty },
    edg { "Midly", midly },
    edg { "Righty", righty },
  };
  static constexpr const trn intro = {
    .from = intro_meta,
    .choices = { intro_choices },
  };

  static constexpr const auto title_choices = std::array {
    edg { nullptr, intro },
  };
  static constexpr const trn title = {
    .from = title_meta,
    .choices = title_choices,
  };

  static constexpr const auto game = title;
}
