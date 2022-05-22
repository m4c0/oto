#define CAML_NAME_SPACE
#include "caml/fail.h"
#include "caml/mlvalues.h"

#include <SDL.h>

extern "C" CAMLprim value peg_init(value) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    caml_failwith("SDL_Init failed");
  }
  return Val_unit;
}
