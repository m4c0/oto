#pragma once

#include "oto/scene.hpp"

namespace poc {
  struct domain {
    enum actor { lefty, midly, midly_hot, righty };
    enum background { title, restaurant, end };
    enum music { romance, game_over };
    enum side { left, middle, right };

    [[nodiscard]] static constexpr const char * actor_to_cstr(actor a) {
      switch (a) {
      case lefty:
        return "lefty";
      case midly:
        return "midly";
      case midly_hot:
        return "midly-hot";
      case righty:
        return "righty";
      }
    }
    [[nodiscard]] static constexpr const char * background_to_cstr(background b) {
      switch (b) {
      case title:
        return "title";
      case restaurant:
        return "restaurant";
      case end:
        return "end";
      }
    }
    [[nodiscard]] static constexpr const char * music_to_cstr(music m) {
      switch (m) {
      case romance:
        return "romance-1";
      case game_over:
        return "game-over";
      }
    }
    [[nodiscard]] static constexpr const char * side_to_cstr(side s) {
      switch (s) {
      case left:
        return "left";
      case middle:
        return "middle";
      case right:
        return "right";
      }
    }
  };
  using lin = oto::line<domain>;
  using osd = std::optional<domain::side>;
  using scm = oto::scene_meta<domain>;

  static constexpr const auto title_meta = scm {
    .name = "Title",
    .background = domain::title,
    .music = domain::romance,
  };
  static constexpr const auto lefty_meta = scm {
    .name = "Lefty Intro",
  };
  static constexpr const auto midly_script = std::array { lin { domain::midly, "This is me" } };
  static constexpr const auto midly_meta = scm {
    .name = "Midly Intro",
    .cast = [](auto /*a*/) -> osd {
      return { domain::left };
    },
    .script = std::span { midly_script },
  };
  static constexpr const auto righty_meta = scm {
    .name = "Righty Intro",
  };

  static constexpr const auto the_end_meta = scm {
    .name = "The End",
    .background = domain::end,
    .music = domain::game_over,
  };

  [[nodiscard]] static constexpr auto restaurant_meta(scm base) {
    base.background = domain::restaurant;
    base.music = domain::romance;
    return base;
  }
  static constexpr const auto intro_script = std::array {
    lin { domain::lefty, "Hello! How are you doing?" },
    lin { domain::lefty, "Do you want to know what's happening?" },
    lin { domain::righty, "Yeah, you! Baby, baby!" },
    lin { domain::midly, "Click one option to select who you want to date" },
    lin { domain::midly_hot, "I'm as hot as an idol" },
  };
  static constexpr const auto intro_meta = restaurant_meta(scm {
      .name = "Intro",
      .cast = [](domain::actor a) -> osd {
        switch (a) {
        case domain::lefty:
          return { domain::left };
        case domain::midly:
        case domain::midly_hot:
          return { domain::middle };
        case domain::righty:
          return { domain::right };
        }
      },
      .script = intro_script,
  });

  //// Transitions ////

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
