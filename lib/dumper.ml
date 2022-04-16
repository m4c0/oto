let rec print_yaml (fn : Types.scene) =
  let print_kv k v =
    print_string k;
    print_string ": ";
    print_endline v
  in
  let meta, next = fn () in
  print_kv "name" meta.name;
  Option.iter (print_kv "background") meta.background;
  Option.iter (print_kv "music") meta.music;
  print_endline "---";
  match next with
  | Continuation s -> print_yaml s
  | WaitThenJump s -> print_yaml s
  | Choice l -> List.iter (fun (_, s) -> print_yaml s) l
  | EndGame -> ()


