module M (P : Printer.M) = struct
  open Oto.Types (P)

  let asset_of_script ({ actor; _ } : opcode) =
    P.actor_to_string actor |> String.cat "char/"

  let rec assets_of (acc : string list) = function
    | [] -> List.sort_uniq String.compare acc
    | sc :: scl -> assets_of_meta acc scl sc

  and assets_of_meta (acc : string list) (scl : scene list) (sc : scene) =
    let ({ background; music; script; _ } : scene_meta), next = sc () in
    let bm fn base opt =
      Option.map fn opt |> Option.map (Filename.concat base)
    in
    let bg = bm P.background_to_string "back" background in
    let music = bm P.music_to_string "music" music in
    let ch = List.map asset_of_script script in
    let res =
      List.map Option.to_list [ bg; music ]
      |> List.flatten |> List.append ch |> List.append acc
    in
    match next with
    | Continuation s | WaitThenJump s -> assets_of_meta res scl s
    | Choice l -> List.map scene_of_choice l |> assets_of res
    | EndGame -> assets_of res scl

  let assets_of_scene s = assets_of [] [ s ]
end
