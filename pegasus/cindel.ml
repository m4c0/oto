open Bigarray

type float_ba = (float, float32_elt, c_layout) Bigarray.Array1.t
type audio_callback = float_ba -> unit

type init_params = {
  width : int;
  height : int;
  audio_callback : audio_callback;
}

type state = { any_key_down : bool }
type renderer
type surface
type texture

external create_empty_texture : renderer -> texture = "peg_create_empty_texture"
external create_texture : renderer -> surface -> texture = "peg_create_texture"
external event_loop : renderer -> (unit -> unit) -> state = "peg_event_loop"
external full_blit : renderer -> texture -> unit = "peg_full_blit"
external init : init_params -> renderer = "peg_init"
external set_audio_callback : audio_callback -> unit = "peg_set_audio_callback"

external create_color_surface : width:int -> height:int -> rgb:int -> surface
  = "peg_create_color_surface"
