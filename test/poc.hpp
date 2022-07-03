#pragma once

#include <array>
#include <optional>
#include <span>
#include <type_traits>

namespace oto {
  template<typename Tp, typename Up>
  concept same_as = std::is_same_v<Tp, Up>;

  template<typename Tp>
  concept domain = requires {
    typename Tp::actor;
    typename Tp::background;
    typename Tp::music;
    typename Tp::side;

    { Tp::actor_to_cstr(typename Tp::actor()) } -> same_as<const char *>;
    { Tp::background_to_cstr(typename Tp::background()) } -> same_as<const char *>;
    { Tp::music_to_cstr(typename Tp::music()) } -> same_as<const char *>;
    { Tp::side_to_cstr(typename Tp::side()) } -> same_as<const char *>;
  };

  template<domain D>
  struct opcode {
    typename D::actor actor;
    const char * line;
  };

  template<domain D>
  struct scene_meta {
    const char * name;
    std::optional<typename D::background> background;
    std::optional<typename D::music> music;
    std::optional<typename D::side> (*cast)(typename D::actor);
    std::span<const opcode<D>> script;
  };

  template<domain D>
  constexpr auto null_cast(typename D::side /*unused*/) {
    return std::nullopt;
  };
}

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
  using opc = oto::opcode<domain>;
  using osd = std::optional<domain::side>;
  using scm = oto::scene_meta<domain>;

  constexpr const auto title_meta = scm {
    .name = "Title",
    .background = domain::title,
    .music = domain::romance,
  };
  constexpr const auto lefty_meta = scm {
    .name = "Lefty Intro",
  };
  constexpr const auto midly_script = std::array { opc { domain::midly, "This is me" } };
  constexpr const auto midly_meta = scm {
    .name = "Midly Intro",
    .cast = [](auto /*a*/) -> osd {
      return { domain::left };
    },
    .script = std::span { midly_script },
  };
  constexpr const auto righty_meta = scm {
    .name = "Righty Intro",
  };

  constexpr const auto the_end_meta = scm {
    .name = "The End",
    .background = domain::end,
    .music = domain::game_over,
  };

  [[nodiscard]] constexpr auto restaurant_meta(scm base) {
    base.background = domain::restaurant;
    base.music = domain::romance;
    return base;
  }
  constexpr const auto intro_script = std::array {
    opc { domain::lefty, "Hello! How are you doing?" },
    opc { domain::lefty, "Do you want to know what's happening?" },
    opc { domain::righty, "Yeah, you! Baby, baby!" },
    opc { domain::midly, "Click one option to select who you want to date" },
    opc { domain::midly_hot, "I'm as hot as an idol" },
  };
  constexpr const auto intro_meta = restaurant_meta(scm {
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
  });
}
