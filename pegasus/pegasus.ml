module Cindel = Cindel

module M (S : Specs.M) = struct
  open S
  module State = State.M (S)

  let present_timeout_ms = 2000
  let ms_per_char = 100

  let run (s : Oto.Types(S.D).scene) =
    let audio_callback data = Bigarray.Array1.fill data 0.0 in
    let renderer = Cindel.init { width; height; audio_callback } in

    let run_actions (s : State.t) (cin : Cindel.state) : State.t =
      let next_with_timer next = State.with_timer next (Ticks.now ()) in
      let wait_keypress next =
        if cin.any_key_down then next_with_timer next else s
      in
      match State.step s with
      | Some (Background b, next) -> State.with_background next b
      | Some (Music m, next) ->
          S.load_music m |> Cindel.set_audio_callback;
          next
      | Some (Pause, next) -> wait_keypress next
      | Some (Present, next) ->
          if State.timer_of_in_ms s > present_timeout_ms then
            next_with_timer next
          else s
      | Some (Speak { text; _ }, next) ->
          let chars = State.timer_of_in_ms s / ms_per_char in
          if chars > String.length text then wait_keypress next
          else if cin.any_key_down then (
            print_endline text;
            State.with_timer s Ticks.epoch)
          else
            let partial = String.sub text 0 chars in
            print_string partial;
            print_char (char_of_int 27);
            print_string "[K";
            print_char '\r';
            flush stdout;
            s
      | Some (Choose _, next) ->
          print_endline "choo choo";
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
