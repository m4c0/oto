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

let from_scene (_ : Types.scene) : t = Seq.empty
