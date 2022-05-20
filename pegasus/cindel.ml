type state = { any_key_down : bool }

let init width height =
  Sdl.init [ `VIDEO ];
  let _, renderer =
    Sdl.Render.create_window_and_renderer ~width ~height ~flags:[]
  in
  renderer

let event_loop renderer =
  let rec loop state =
    match Sdl.Event.poll_event () with
    | None ->
        Sdl.Render.set_draw_color renderer ~rgb:(255, 0, 255) ~a:255;
        Sdl.Render.clear renderer;
        Sdl.Render.render_present renderer;
        state
    | Some (Sdl.Event.Quit _) ->
        Sdl.quit ();
        exit 0
    | Some (Sdl.Event.KeyUp _) -> loop { any_key_down = true }
    | Some _ -> loop state
  in
  loop { any_key_down = false }
