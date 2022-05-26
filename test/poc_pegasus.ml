module P = Pegasus.M (struct
  module D = Poc.Printer

  let width = 800
  let height = 600

  let load_background (bg : D.background) =
    let rgb =
      match bg with Title -> 0xFF00 | Restaurant -> 0xFF0000 | End -> 0xFF
    in
    Pegasus.Cindel.create_color_surface ~width ~height ~rgb

  let load_music (mus : D.music) =
    let mult = match mus with Romance -> 1. | GameOver -> 2. in
    let music_ref = ref 0.0 in
    fun data ->
      let open Bigarray in
      let size = Array1.dim data in
      let rec fill n p =
        if n == size then music_ref := p
        else (
          Array1.set data n (sin p *. 0.25 *. mult);
          fill (n + 1) (p +. 0.05))
      in
      fill 0 !music_ref
end)

let () = P.run Poc.game
