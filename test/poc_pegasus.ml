module P = Pegasus.M (struct
  module D = Poc.Printer

  let width = 800
  let height = 600

  let load_background (bg : D.background) =
    let rgb =
      match bg with Title -> 0xFF00 | Restaurant -> 0xFF0000 | End -> 0xFF
    in
    Pegasus.Cindel.create_color_surface ~width ~height ~rgb
end)

let () = P.run Poc.game
