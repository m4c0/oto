module Cindel = Cindel

module M (S : Specs.M) = struct
  open S
  open Oto.Types (D)
  open Oto.Vm (D)
  module State = State.M (S)

  let present_timeout_ms = 2000

  let run (s : scene) =
    let audio_callback data = Bigarray.Array1.fill data 0.0 in
    let renderer = Cindel.init { width; height; audio_callback } in

    let run_action : action -> unit = function
      | Choose _ -> print_endline "choo choo"
      | Speak _ -> print_endline "woof"
      | _ -> failwith "Done already"
    in
    let run_actions (s : State.t) (cin : Cindel.state) : State.t =
      let timer = Ticks.now () in
      match State.step s with
      | Some (Background b, next) -> State.with_background next b
      | Some (Music m, next) ->
          S.load_music m |> Cindel.set_audio_callback;
          next
      | Some (Pause, next) -> if cin.any_key_down then next else s
      | Some (Present, next) ->
          let ms = State.timer_of s |> Ticks.ms_from_now in
          if ms > present_timeout_ms then State.with_timer next timer else s
      | Some (a, next) ->
          run_action a;
          next
      | None -> s
    in

    let draw wnd _ = State.background_of wnd |> Cindel.full_blit renderer in

    let rec main_loop wnd =
      let state = Cindel.event_loop renderer (draw wnd) in
      run_actions wnd state |> main_loop
    in
    State.create renderer s |> main_loop
end
