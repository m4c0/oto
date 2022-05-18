module M (P : Printer.M) = struct
  module O = Oto.M (P)

  let rec print_command = function
    | [] -> print_newline ()
    | [ x ] -> print_endline x
    | x :: xs ->
        print_string x;
        print_string " ";
        print_command xs

  let str_from_side (s : O.side) =
    match s with Left -> "left" | Right -> "right" | Middle -> "middle"

  let do_action (a : O.action) =
    match a with
    | Background b -> print_command [ "background"; b ]
    | Choose _ -> ()
    | Music b -> print_command [ "music"; b ]
    | Pause -> print_endline "pause"
    | Present -> print_endline "present"
    | Speak { side; actor; pose; text } ->
        print_command
          [
            "speak";
            str_from_side side;
            P.actor_to_string actor;
            Option.value ~default:"none" pose;
            text;
          ]

  let rec run chooser (vm : O.t) =
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

  let print_actions chooser scene = run chooser @@ O.from_scene scene
end
