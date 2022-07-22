#include "oto/renderer.hpp"

namespace oto::r {
  struct texture_ptr {
    void * ptr;
    int w, h;
  };
}

extern "C" void oto_draw(void * ptr, int w, int h);

void oto::r::init(void * /**/) {
}

void oto::r::deleter::operator()(oto::r::texture_ptr * ptr) const {
  free(ptr->ptr);
  delete ptr;
}

oto::texture oto::r::create_rgba_texture(int w, int h, std::span<const unsigned> data) {
  auto size = w * h * sizeof(unsigned);
  void * res = malloc(size);
  memcpy(res, data.begin(), size);
  return oto::texture(new texture_ptr { res, w, h });
}

oto::texture oto::r::create_color_texture(int w, int h, unsigned color) {
  auto size = w * h;
  void * res = malloc(size * sizeof(unsigned));
  unsigned * ptr = static_cast<unsigned *>(res);
  while (size--)
    *ptr++ = color;
  return oto::texture(new texture_ptr { res, w, h });
}

void oto::r::draw(const oto::texture & txt) {
  oto_draw(txt->ptr, txt->w, txt->h);
}

void oto::r::draw(const oto::texture & txt, const oto::rect & rect) {
}

void oto::r::draw(const oto::texture & txt, const oto::rect & clip, const oto::rect & target) {
}
