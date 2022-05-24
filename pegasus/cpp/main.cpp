#include <SDL.h>
#include <cstdlib>

#define CAML_NAME_SPACE
#include "caml/alloc.h"
#include "caml/callback.h"
#include "caml/custom.h"
#include "caml/fail.h"
#include "caml/gc.h"
#include "caml/memory.h"
#include "caml/mlvalues.h"

static int l = 0;
extern "C" CAMLprim value peg_init(value params) {
  CAMLparam1(params);

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    caml_failwith("SDL_Init failed");
  }

  enum { width = 0, height, callback };
  int w = Field(params, width);
  int h = Field(params, height);

  SDL_Window *window;
  SDL_Renderer *renderer;
  if (SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer) < 0) {
    caml_failwith("SDL_CreateWindowAndRenderer failed");
  }

  SDL_AudioSpec desired{
      .freq = 44100,
      .format = AUDIO_F32,
      .channels = 1,
      .samples = 4096,
      .callback =
          [](void *data, Uint8 *str, int len) {
            float *f = reinterpret_cast<float *>(str);
            int lf = len / sizeof(float);
            for (int i = 0; i < lf; i++) {
              f[i] = sin((float)(l + i) * 0.05) * 0.25;
            };
            l += lf;
          },
  };
  SDL_AudioSpec obtained;
  auto dev = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, 0);

  SDL_PauseAudioDevice(dev, 0);

  CAMLreturn(reinterpret_cast<value>(renderer));
}

static custom_operations _texture_co{
    .identifier = "SDL_Texture",
    .finalize =
        [](value v) {
          auto t = *reinterpret_cast<SDL_Texture **>(Data_custom_val(v));
          SDL_DestroyTexture(t);
        },
    .compare = custom_compare_default,
    .hash = custom_hash_default,
    .serialize = custom_serialize_default,
    .deserialize = custom_deserialize_default,
    .compare_ext = custom_compare_ext_default,
    .fixed_length = custom_fixed_length_default,
};
static value _create_texture(SDL_Renderer *rnd, SDL_Surface *surf) {
  value res = caml_alloc_custom(&_texture_co, sizeof(SDL_Texture *), 0, 1);
  *reinterpret_cast<SDL_Texture **>(Data_custom_val(res)) =
      SDL_CreateTextureFromSurface(rnd, surf);
  return res;
}
extern "C" CAMLprim value peg_create_texture(SDL_Renderer *rnd,
                                             SDL_Surface *surf) {
  CAMLparam0();
  CAMLreturn(_create_texture(rnd, surf));
}

extern "C" CAMLprim value peg_create_empty_texture(SDL_Renderer *rnd) {
  CAMLparam0();

  SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormat(
      0, 16, 16, 24, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGB888);
  auto res = _create_texture(rnd, surf);
  SDL_FreeSurface(surf);

  CAMLreturn(res);
}

extern "C" CAMLprim void peg_full_blit(SDL_Renderer *rnd, value txt) {
  CAMLparam1(txt);

  auto t = *reinterpret_cast<SDL_Texture **>(Data_custom_val(txt));
  if (SDL_RenderCopy(rnd, t, NULL, NULL) < 0) {
    caml_failwith("SDL_RenderCopy failed");
  }

  CAMLreturn0;
}

extern "C" CAMLprim value peg_event_loop(SDL_Renderer *rnd, value fn) {
  CAMLparam1(fn);
  CAMLlocal1(res);

  enum res_fields { any_key_down = 0 };

  res = caml_alloc(1, 0);
  Store_field(res, any_key_down, Val_false);

  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_KEYUP:
      Store_field(res, any_key_down, Val_true);
      break;
    case SDL_QUIT:
      SDL_Quit();
      exit(0);
      break;
    }
  }

  SDL_SetRenderDrawColor(rnd, 255, 0, 255, 255);
  SDL_RenderClear(rnd);
  caml_callback(fn, Val_unit);
  SDL_RenderPresent(rnd);

  CAMLreturn(res);
}

extern "C" CAMLprim SDL_Surface *peg_create_color_surface(int w, int h,
                                                          int rgb) {
  SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormat(
      0, w, h, 24, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGB888);

  SDL_Rect rect{0, 0, w, h};
  SDL_FillRect(surf, &rect, rgb);
  return surf;
}
