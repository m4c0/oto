module S (D : Oto.Domain) = struct
  let width = 800
  let height = 600

  let load_background _ =
    let color : int32 = Int32.of_int 0xFFFFFF in
    let rect = Sdl.Rect.make ~pos:(0, 0) ~dims:(width, height) in
    let dst = Sdl.Surface.create_rgb ~width ~height ~depth:24 in
    Sdl.Surface.fill_rect ~color ~dst ~rect;
    dst
end

module P = Pegasus.M (Poc.Printer) (S)

let () = P.run Poc.game
