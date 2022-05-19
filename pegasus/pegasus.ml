module type Specs = sig
  val width : int
  val height : int
end

module M (S : Specs) (D : Oto.Domain) = struct
  let clear_color = (0, 0, 0)

  let run (_ : Oto.M(D).scene) =
    Sdl.init [ `VIDEO ];
    let _, renderer =
      Sdl.Render.create_window_and_renderer ~width:S.width ~height:S.height
        ~flags:[]
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
end
