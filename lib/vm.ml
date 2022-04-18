type side = Left | Middle | Right
type line = { side : side; actor : string; pose : string option; text : string }

type action =
  | Background of string
  | Cast of string list
  | Choose of string list
  | Music of string
  | Pause
  | Speak of line

type t = action Seq.t

let background x = Background x
let music x = Music x

let from_scene (s : Types.scene) : t =
  let meta, _ = s () in
  let cmd_from_opt c o = Option.map c o |> Option.to_seq in
  let bg = cmd_from_opt background meta.background in
  let m = cmd_from_opt music meta.music in
  Seq.append bg m
