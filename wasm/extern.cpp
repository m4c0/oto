#include "oto/renderer.hpp"

void oto::r::init(void * /**/) {
}

void oto::r::deleter::operator()(oto::r::texture_ptr *) const {
}

oto::texture oto::r::create_rgba_texture(int w, int h, std::span<const unsigned> data) {
  return {};
}

oto::texture oto::r::create_color_texture(int w, int h, unsigned color) {
  return {};
}

void oto::r::draw(const oto::texture & txt) {
}

void oto::r::draw(const oto::texture & txt, const oto::rect & rect) {
}

void oto::r::draw(const oto::texture & txt, const oto::rect & clip, const oto::rect & target) {
}

void oto::r::draw_string(const oto::texture & font, const oto::size & chr_size, std::string_view str, int x, int y) {
}
