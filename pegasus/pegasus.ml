module type Specs = sig
  val width : int
  val height : int
end

module M (S : Specs) (D : Oto.Domain) = struct
  module Oto = Oto.M (D)

  let run (s : Oto.scene) =
    let renderer = Cindel.init S.width S.height in

    let rec main_loop vm =
      Cindel.event_loop renderer;
      main_loop vm
    in
    Oto.from_scene s |> main_loop
end
