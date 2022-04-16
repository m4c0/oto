let rec print_yaml (fn : Types.scene) =
  let print_kv k v =
    print_string k;
    print_string ": ";
    print_endline v
  in
  let print_val p v =
    print_string p;
    print_endline v
  in
  let print_cast lbl = function
    | [] -> ()
    | l ->
        print_string "  ";
        print_string lbl;
        print_endline ":";
        List.iter (print_val "    - ") l
  in
  let meta, next = fn () in
  let cast = meta.actors () in
  print_kv "name" meta.name;
  Option.iter (print_kv "background") meta.background;
  Option.iter (print_kv "music") meta.music;
  print_endline "actors:";
  print_cast "left" cast.left;
  print_cast "middle" cast.middle;
  print_cast "right" cast.right;
  print_endline "---";
  match next with
  | Continuation s -> print_yaml s
  | WaitThenJump s -> print_yaml s
  | Choice l -> List.iter (fun (_, s) -> print_yaml s) l
  | EndGame -> ()
