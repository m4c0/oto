let width = 640
let height = 480
let flags = []
let clear_color = (0, 0, 0)

let run () =
  Sdl.init [ `VIDEO ];
  let _, renderer =
    Sdl.Render.create_window_and_renderer ~width ~height ~flags
  in

  let dispatch = function
    | Sdl.Event.Quit _ ->
        Sdl.quit ();
        exit 0
    | _ -> ()
  in
  let rec event_loop () =
    match Sdl.Event.poll_event () with
    | None -> ()
    | Some e ->
        dispatch e;
        event_loop ()
  in
  let render () =
    Sdl.Render.set_draw_color renderer ~rgb:clear_color ~a:255;
    Sdl.Render.clear renderer;
    Sdl.Render.render_present renderer
  in
  let rec main_loop () =
    event_loop ();
    render ();
    main_loop ()
  in
  main_loop ()
