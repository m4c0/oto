module Cindel = Cindel

module M (S : sig
  module D : Oto.Domain

  val width : int
  val height : int
  val load_background : D.background -> Cindel.surface
end) =
struct
  open S
  open Oto.Types (D)
  open Oto.Vm (D)

  type window = { background : Cindel.texture; timer : Ticks.t; vm : t }

  let present_timeout_ms = 2000

  let run (s : scene) =
    let music_ref = ref 0.0 in
    let music_callback data =
      let open Bigarray in
      let size = Array1.dim data in
      let rec fill n p =
        if n == size then music_ref := p
        else (
          Array1.set data n (sin p *. 0.25);
          fill (n + 1) (p +. 0.05))
      in
      fill 0 !music_ref
    in
    let renderer = Cindel.init { width; height; music_callback } in

    let run_action : action -> unit = function
      | Choose _ -> print_endline "choo choo"
      | Music _ -> print_endline "mus"
      | Speak _ -> print_endline "woof"
      | _ -> failwith "Done already"
    in
    let run_actions (wnd : window) (cin : Cindel.state) =
      let timer = Ticks.now () in
      match wnd.vm () with
      | Cons (Background b, next) ->
          {
            background = load_background b |> Cindel.create_texture renderer;
            timer;
            vm = next;
          }
      | Cons (Pause, next) ->
          if cin.any_key_down then { wnd with vm = next } else wnd
      | Cons (Present, next) ->
          let ms = Ticks.ms_from_now wnd.timer in
          if ms > present_timeout_ms then { wnd with vm = next; timer } else wnd
      | Cons (a, next) ->
          run_action a;
          { wnd with vm = next }
      | Nil -> wnd
    in

    let draw wnd _ = Cindel.full_blit renderer wnd.background in

    let rec main_loop wnd =
      let state = Cindel.event_loop renderer (draw wnd) in
      run_actions wnd state |> main_loop
    in
    {
      background = Cindel.create_empty_texture renderer;
      timer = Ticks.now ();
      vm = from_scene s;
    }
    |> main_loop
end
