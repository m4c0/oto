#define CAML_NAME_SPACE
#include "caml/mlvalues.h"

extern "C" CAMLprim int foo_echo(int v) { return v + 1; }
