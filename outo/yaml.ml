module M (P : Printer.M) = struct
  open Oto.Types (P)

  (* TODO: check for scene equality *)
  module StrSet = Set.Make (String)

  let print_yaml (fn : scene) =
    let done_set = ref StrSet.empty in
    let print_kv k v =
      print_string k;
      print_string ": ";
      print_endline v
    in
    let print_cast ({ cast; script; _ } : scene_meta) =
      let op_to_str ({ actor; _ } : opcode) =
        let open Oto.Vm (P) in
        let a = P.actor_to_string actor in
        match cast actor |> Option.map P.side_to_string with
        | None -> raise ActorNotFoundInCast
        | Some s -> "  " ^ a ^ ": " ^ s
      in
      List.map op_to_str script
      |> List.sort_uniq String.compare
      |> List.iter print_endline
    in
    let print_script ({ actor; line } : opcode) =
      print_string "  - [";
      P.actor_to_string actor |> print_string;
      print_string "] ";
      print_endline line
    in
    let print_title (s : scene) =
      let meta, _ = s () in
      print_endline meta.name
    in
    let print_choice (n, (s : scene)) =
      let meta, _ = s () in
      print_string "  - \"";
      print_string n;
      print_string "\" jumps to ";
      print_endline meta.name
    in
    let rec run_if_new (s : scene) =
      let meta, _ = s () in
      match StrSet.find_opt meta.name !done_set with
      | None ->
          done_set := StrSet.add meta.name !done_set;
          print_yaml_impl s
      | _ -> ()
    and print_yaml_impl (fn : scene) =
      let meta, next = fn () in
      let bm fn base opt = Option.map fn opt |> Option.iter (print_kv base) in
      print_kv "name" meta.name;
      bm P.background_to_string "background" meta.background;
      bm P.music_to_string "music" meta.music;
      print_endline "actors:";
      print_cast meta;
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
          print_endline "outcome:";
          List.iter print_choice l;
          print_endline "---";
          List.iter (fun (_, s) -> run_if_new s) l
      | EndGame ->
          print_endline "outcome: end game";
          print_endline "---"
    in
    print_yaml_impl fn
end
