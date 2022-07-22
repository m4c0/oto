#include "oto/renderer.hpp"

void oto::r::init(void * /**/) {
}

void oto::r::deleter::operator()(oto::r::texture_ptr * ptr) const {
  free(reinterpret_cast<unsigned *>(ptr));
}

oto::texture oto::r::create_rgba_texture(int w, int h, std::span<const unsigned> data) {
  auto size = w * h * sizeof(unsigned);
  void * res = malloc(size);
  memcpy(res, data.begin(), size);
  return oto::texture(reinterpret_cast<oto::r::texture_ptr *>(res));
}

oto::texture oto::r::create_color_texture(int w, int h, unsigned color) {
  auto size = w * h;
  void * res = malloc(size * sizeof(unsigned));
  unsigned * ptr = static_cast<unsigned *>(res);
  while (size--)
    *ptr++ = color;
  return oto::texture(reinterpret_cast<oto::r::texture_ptr *>(res));
}

void oto::r::draw(const oto::texture & txt) {
}

void oto::r::draw(const oto::texture & txt, const oto::rect & rect) {
}

void oto::r::draw(const oto::texture & txt, const oto::rect & clip, const oto::rect & target) {
}
