module Cindel = Cindel

module M (S : sig
  module D : Oto.Domain

  val width : int
  val height : int
  val load_background : D.background -> Cindel.surface
  val load_music : D.music -> Cindel.audio_callback
end) =
struct
  open S
  open Oto.Types (D)
  open Oto.Vm (D)

  type window = { background : Cindel.texture; timer : Ticks.t; vm : t }

  let present_timeout_ms = 2000

  let run (s : scene) =
    let audio_callback data = Bigarray.Array1.fill data 0.0 in
    let renderer = Cindel.init { width; height; audio_callback } in

    let run_action : action -> unit = function
      | Choose _ -> print_endline "choo choo"
      | Speak _ -> print_endline "woof"
      | _ -> failwith "Done already"
    in
    let run_actions (wnd : window) (cin : Cindel.state) =
      let timer = Ticks.now () in
      match wnd.vm () with
      | Cons (Background b, next) ->
          {
            wnd with
            background = load_background b |> Cindel.create_texture renderer;
            vm = next;
          }
      | Cons (Music m, next) ->
          S.load_music m |> Cindel.set_audio_callback;
          { wnd with vm = next }
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
