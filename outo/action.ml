module M (P : Printer.M) = struct
  open Oto.Builder (P)
  open Oto.Vm (P)

  let rec print_command = function
    | [] -> print_newline ()
    | [ x ] -> print_endline x
    | x :: xs ->
        print_string x;
        print_string " ";
        print_command xs

  let do_action (a : action) =
    match a with
    | Background b -> print_command [ "background"; P.background_to_string b ]
    | Choose _ -> ()
    | Music b -> print_command [ "music"; P.music_to_string b ]
    | Pause -> print_endline "pause"
    | Present -> print_endline "present"
    | Speak { side; actor; text } ->
        print_command
          [ "speak"; P.side_to_string side; P.actor_to_string actor; text ]

  let rec run chooser (vm : t) =
    match vm () with
    | Nil -> print_endline "----"
    | Cons (Choose b, _) ->
        let cb = List.map (fun (c, _) -> c) b in
        print_command ("choose" :: cb);
        print_endline "----";
        chooser b |> fun (_, nvm) -> run chooser nvm
    | Cons (a, nvm) ->
        do_action a;
        run chooser nvm

  let print_actions chooser scene = run chooser @@ from_scene scene
end
