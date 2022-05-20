module type Specs = functor (D : Oto.Domain) -> sig
  open D

  val width : int
  val height : int
  val load_background : background -> Sdl.Surface.t
end

module M (D : Oto.Domain) (S : Specs) = struct
  open S (D)
  open Oto.M (D)

  type window = { background : Sdl.Texture.t; vm : t }

  let run (s : scene) =
    let renderer = Cindel.init width height in

    let run_action : action -> unit = function
      | Choose _ -> print_endline "choo choo"
      | Music _ -> print_endline "mus"
      | Speak _ -> print_endline "woof"
      | _ -> failwith "Done already"
    in
    let run_actions (wnd : window) (s : Cindel.state) =
      match wnd.vm () with
      | Cons (Background b, next) ->
          {
            background = load_background b |> Cindel.create_texture renderer;
            vm = next;
          }
      | Cons (Pause, next) ->
          if s.any_key_down then { wnd with vm = next } else wnd
      | Cons (Present, next) ->
          (* TODO: timer *)
          if s.any_key_down then { wnd with vm = next } else wnd
      | Cons (a, next) ->
          run_action a;
          { wnd with vm = next }
      | Nil -> wnd
    in

    let draw wnd = Sdl.Render.copy ~texture:wnd.background renderer in

    let rec main_loop wnd =
      let state = Cindel.event_loop renderer (draw wnd) in
      run_actions wnd state |> main_loop
    in
    { background = Cindel.create_empty_texture renderer; vm = from_scene s }
    |> main_loop
end
