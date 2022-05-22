type t = int

let now _ = Sdl.Timer.get_ticks ()
let ms_from_now (n : t) = now () - n
