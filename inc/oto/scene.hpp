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
  struct line {
    typename D::actor actor;
    const char * text;
  };

  template<domain D>
  struct scene_meta {
    const char * name;
    std::optional<typename D::background> background;
    std::optional<typename D::music> music;
    std::optional<typename D::side> (*cast)(typename D::actor);
    std::span<const line<D>> script;
  };

  template<domain D>
  constexpr auto null_cast(typename D::side /*unused*/) {
    return std::nullopt;
  };
}
