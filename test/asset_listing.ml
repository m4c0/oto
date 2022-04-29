let print_assets (s : Oto.Types.scene) =
  let ({ background; music; _ } : Oto.Types.scene_meta), _ = s () in
  let print_asset tp file = Filename.concat tp file |> print_endline in
  let print_opt_asset tp file = Option.iter (print_asset tp) file in
  print_opt_asset "back" background;
  print_opt_asset "music" music

let () = print_assets Poc.game
