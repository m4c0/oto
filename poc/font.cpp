#include "font.hpp"

#include <array>
#include <string_view>

static constexpr const auto max_chars = 256;
static constexpr const auto char_cols = 4;
static constexpr const auto char_rows = 5;
static constexpr const auto chars = [] {
  std::array<std::string_view, max_chars> ascii {};

  ascii['e'] = "XXX "
               "X   "
               "XXX "
               "X   "
               "XXX ";

  ascii['h'] = "X X "
               "X X "
               "XXX "
               "X X "
               "X X ";

  ascii['l'] = "X   "
               "X   "
               "X   "
               "X   "
               "XXX ";

  ascii['o'] = "XXX "
               "X X "
               "X X "
               "X X "
               "XXX ";

  for (auto c = 'a', d = 'A'; c <= 'z'; c++, d++) {
    ascii.at(d) = ascii.at(c);
  }

  return ascii;
}();

static constexpr const auto xmap_stride = max_chars * char_cols;
static constexpr const auto xmap_rows = 8; // PoT
static constexpr const auto xmap_size = xmap_rows * xmap_stride;
static constexpr const auto xmap = [] {
  std::array<unsigned, xmap_size> map {};

  for (int c = 0; c < max_chars; c++) {
    auto cs = chars.at(c);
    if (cs.empty()) continue;
    for (int y = 0, p = 0; y < char_rows; y++) {
      for (int x = 0; x < char_cols; x++, p++) {
        auto idx = c * char_cols + x + y * xmap_stride;
        map.at(idx) = cs.at(p) == 'X' ? ~0U : 0U;
      }
    }
  }

  return map;
}();
static_assert(xmap_rows >= char_rows);

static constexpr const auto bitmap_size_factor = 4;
static constexpr const auto bitmap_stride = xmap_stride * bitmap_size_factor;
static constexpr const auto bitmap_rows = xmap_rows * bitmap_size_factor;
static constexpr const auto bitmap_size = bitmap_stride * bitmap_rows;

oto::texture poc::load_text_font() {
  std::array<unsigned, bitmap_size> bmap {};
  for (auto i = 0; i < bitmap_size; i++) {
    auto x = (i % bitmap_stride) / bitmap_size_factor;
    auto y = (i / bitmap_stride) / bitmap_size_factor;
    bmap.at(i) = xmap.at(y * xmap_stride + x);
  }

  return oto::r::create_rgba_texture(bitmap_stride, bitmap_rows, std::span<const unsigned> { bmap });
}
