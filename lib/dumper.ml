module StrSet = Set.Make (String)

let print_yaml (fn : Types.scene) =
  let done_set = ref StrSet.empty in
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
  let print_script (scr : Types.opcode) =
    match scr with
    | Speech (actor, line) ->
        print_string "  - [";
        print_string actor;
        print_string "] ";
        print_endline line
    | Pose (actor, pose) ->
        print_string "  - ";
        print_string actor;
        print_string " poses ";
        print_endline pose
  in
  let print_title (s : Types.scene) =
    let meta, _ = s () in
    print_endline meta.name
  in
  let rec run_if_new (s : Types.scene) =
    let meta, _ = s () in
    match StrSet.find_opt meta.name !done_set with
    | None -> 
        done_set := StrSet.add meta.name !done_set;
        print_yaml_impl s
    | _ -> ()
  and print_yaml_impl (fn : Types.scene) =
    let meta, next = fn () in
    let cast = meta.actors () in
    print_kv "name" meta.name;
    Option.iter (print_kv "background") meta.background;
    Option.iter (print_kv "music") meta.music;
    print_endline "actors:";
    print_cast "left" cast.left;
    print_cast "middle" cast.middle;
    print_cast "right" cast.right;
    print_endline "script:";
    List.iter print_script meta.script;
    match next with
    | Continuation s ->
        print_string "outcome: jump to ";
        print_title s;
        print_endline "---";
        run_if_new s
    | WaitThenJump s ->
        print_string "outcome: pause then move to ";
        print_title s;
        print_endline "---";
        run_if_new s
    | Choice l ->
        print_endline "---";
        List.iter (fun (_, s) -> run_if_new s) l
    | EndGame ->
        print_endline "outcome: end game";
        print_endline "---"
  in
  print_yaml_impl fn
