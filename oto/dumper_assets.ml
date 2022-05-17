module M (P : Dumper_printer.M) = struct
  let asset_of_script ({ actor; pose; _ } : 'a Types.opcode) =
    let act = P.actor_to_string actor in
    let ch = match pose with None -> act | Some p -> act ^ "-" ^ p in
    "char/" ^ ch

  let rec assets_of (acc : string list) = function
    | [] -> List.sort_uniq String.compare acc
    | sc :: scl -> assets_of_meta acc scl sc

  and assets_of_meta (acc : string list) (scl : 'a Types.scene list)
      (sc : _ Types.scene) =
    let ({ background; music; script; _ } : 'a Types.scene_meta), next =
      sc ()
    in
    let bg = Option.map (Filename.concat "back") background in
    let music = Option.map (Filename.concat "music") music in
    let ch = List.map asset_of_script script in
    let res =
      List.map Option.to_list [ bg; music ]
      |> List.flatten |> List.append ch |> List.append acc
    in
    match next with
    | Continuation s | WaitThenJump s -> assets_of_meta res scl s
    | Choice l -> List.map Types.scene_of_choice l |> assets_of res
    | EndGame -> assets_of res scl

  let assets_of_scene s = assets_of [] [ s ]
end
