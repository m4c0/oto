module type Specs = sig
  val width : int
  val height : int
end

module M (S : Specs) (D : Oto.Domain) = struct
  module Oto = Oto.M (D)

  let run (s : Oto.scene) =
    let renderer = Cindel.init S.width S.height in

    let run_action : Oto.action -> unit = function
      | Background _ -> print_endline "bg"
      | Choose _ -> print_endline "choo choo"
      | Music _ -> print_endline "mus"
      | Pause -> print_endline "pause"
      | Present -> print_endline "present"
      | Speak _ -> print_endline "woof"
    in
    let run_actions (vm : Oto.t) (s : Cindel.state) =
      match vm () with
      | Cons (Present, next) -> if s.any_key_down then next else vm
      | Cons (a, next) ->
          run_action a;
          next
      | Nil -> vm
    in

    let rec main_loop vm =
      let state = Cindel.event_loop renderer in
      run_actions vm state |> main_loop
    in
    Oto.from_scene s |> main_loop
end
