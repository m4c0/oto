module P =
  Pegasus.M
    (Poc.Printer)
    (struct
      open Poc.Printer

      let width = 800
      let height = 600

      let load_background (bg : background) =
        let cint =
          match bg with Title -> 0xFF00 | Restaurant -> 0xFF0000 | End -> 0xFF
        in
        let color : int32 = Int32.of_int cint in
        let rect = Sdl.Rect.make ~pos:(0, 0) ~dims:(width, height) in
        let dst = Sdl.Surface.create_rgb ~width ~height ~depth:24 in
        Sdl.Surface.fill_rect ~color ~dst ~rect;
        dst
    end)

let () = P.run Poc.game
