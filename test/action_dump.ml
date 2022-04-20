let rec print_command = function
  | [] -> print_newline ()
  | [ x ] -> print_endline x
  | x :: xs ->
      print_string x;
      print_string " ";
      print_command xs

let str_from_side (s : Oto.Vm.side) =
  match s with Left -> "left" | Right -> "right" | Middle -> "middle"

let do_action (a : Oto.Vm.action) =
  match a with
  | Background b -> print_command [ "background"; b ]
  | Cast b -> print_command ("cast" :: b)
  | Choose _ -> ()
  | Music b -> print_command [ "music"; b ]
  | Pause -> print_endline "end"
  | Present -> print_endline "present"
  | Speak { side; actor; pose; text } ->
      print_command
        [
          "speak";
          str_from_side side;
          actor;
          Option.value ~default:"none" pose;
          text;
        ]

let rec run (vm : Oto.Vm.t) =
  match vm () with
  | Nil -> print_endline "----"
  | Cons (Choose b, _) ->
      let cb = List.map (fun (c, _) -> c) b in
      print_command ("choose" :: cb);
      List.tl b |> List.hd |> fun (_, nvm) -> run nvm
  | Cons (a, nvm) ->
      do_action a;
      run nvm

let () = run @@ Oto.Vm.from_scene Poc.game
