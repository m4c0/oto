let init width height =
  Sdl.init [ `VIDEO ];
  let _, renderer =
    Sdl.Render.create_window_and_renderer ~width ~height ~flags:[]
  in
  renderer

let rec event_loop renderer =
  match Sdl.Event.poll_event () with
  | None ->
      Sdl.Render.set_draw_color renderer ~rgb:(255, 0, 255) ~a:255;
      Sdl.Render.clear renderer;
      Sdl.Render.render_present renderer
  | Some (Sdl.Event.Quit _) ->
      Sdl.quit ();
      exit 0
  | Some _ -> event_loop renderer
