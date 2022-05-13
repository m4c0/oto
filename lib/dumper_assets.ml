let asset_of_script ({ actor; pose; _ } : Types.opcode) =
  let ch = match pose with None -> actor | Some p -> actor ^ "-" ^ p in
  "char/" ^ ch

let rec assets_of (acc : string list) = function
  | [] -> List.sort_uniq String.compare acc
  | sc :: scl -> assets_of_meta acc scl sc

and assets_of_meta (acc : string list) (scl : Types.scene list)
    (sc : Types.scene) =
  let ({ background; music; script; _ } : Types.scene_meta), next = sc () in
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
