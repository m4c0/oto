#pragma once

#include "font.hpp"
#include "main.hpp"
#include "oto/renderer.hpp"

#include <cmath>

static constexpr const auto HALF_VALUE = 128;

static float p = 0;
static float mult = 1.0;
static void music_callback(std::span<float> data) {
  float mult = 1.0;
  for (int i = 0; i < data.size(); i++) {
    data[i] = sin(p * 0.25 * mult);
    p += 0.05;
  }
}

static constexpr auto act_name(poc::domain::actor act) {
  switch (act) {
  case poc::domain::lefty:
    return "Lefty";
  case poc::domain::midly:
  case poc::domain::midly_hot:
    return "Midly";
  case poc::domain::righty:
    return "Righty";
  }
}
static constexpr unsigned act_color(poc::domain::actor act) {
  switch (act) {
  case poc::domain::lefty:
    return 0xaa00; // NOLINT
  case poc::domain::midly:
    return 0xaa0000; // NOLINT
  case poc::domain::midly_hot:
    return 0xff0000; // NOLINT
  case poc::domain::righty:
    return 0xaa000000; // NOLINT
  }
}
static constexpr unsigned bg_color(poc::domain::background bck) {
  switch (bck) {
  case poc::domain::title:
    return 0x7700; // NOLINT
  case poc::domain::restaurant:
    return 0x770000; // NOLINT
  case poc::domain::end:
    return 0x77000000; // NOLINT
  }
}
struct asset {
  static const char * actor_name(poc::domain::actor act) {
    return act_name(act);
  }
  static oto::texture load_actor(poc::domain::actor act) {
    return oto::r::create_color_texture(HALF_VALUE, HALF_VALUE, act_color(act));
  }
  static oto::texture load_background(poc::domain::background bck) {
    return oto::r::create_color_texture(HALF_VALUE, HALF_VALUE, bg_color(bck));
  }
  static auto load_music(poc::domain::music mus) {
    switch (mus) {
    case poc::domain::romance:
      mult = 1.0f;
      break;
    case poc::domain::game_over:
      mult = 2.0f;
      break;
    }
    return music_callback;
  }
  static oto::texture load_text_background(poc::domain::background bck) {
    if (bck == poc::domain::restaurant) {
      return oto::r::create_color_texture(HALF_VALUE, HALF_VALUE, 0);
    }
    return nullptr;
  }
  static oto::texture load_text_font(poc::domain::background /**/) {
    return poc::load_text_font();
  }
  static auto rect_of_side(poc::domain::side side) {
    static constexpr const auto ACTOR_H = 400;
    static constexpr const auto ACTOR_W = 160;
    static constexpr const auto ACTOR_Y = 100;

    switch (side) {
    case poc::domain::left:
      return oto::rect { ACTOR_W * 1, ACTOR_Y, ACTOR_W, ACTOR_H };
    case poc::domain::middle:
      return oto::rect { ACTOR_W * 2, ACTOR_Y, ACTOR_W, ACTOR_H };
    case poc::domain::right:
      return oto::rect { ACTOR_W * 3, ACTOR_Y, ACTOR_W, ACTOR_H };
    }
  }
  static auto size_of_font(poc::domain::background /**/) {
    return oto::size { .w = 16, .h = 32 };
  }
};
