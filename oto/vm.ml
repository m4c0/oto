type side = Left | Middle | Right
type 'a line = { side : side; actor : 'a; pose : string option; text : string }

type 'a action =
  | Background of string
  | Choose of (string * 'a t) list
  | Music of string
  | Pause
  | Present
  | Speak of 'a line

and 'a t = 'a action Seq.t

exception ActorNotFoundInCast

let background x = Background x
let music x = Music x
let cmd_from_opt c o = Option.map c o |> Option.to_seq

let side_of actor ({ left; middle; right } : 'a Types.cast) =
  let eq a = actor == a in
  let exists = List.exists eq in
  if exists left then Left
  else if exists middle then Middle
  else if exists right then Right
  else raise ActorNotFoundInCast

let opcode_action cast ({ actor; pose; line } : 'a Types.opcode) =
  let side = side_of actor cast in
  Speak { side; actor; pose; text = line }

let rec from_scene (s : 'a Types.scene) : 'a t =
  let meta, transition = s () in
  let bg = cmd_from_opt background meta.background in
  let m = cmd_from_opt music meta.music in
  let act = meta.actors () in
  let scr = List.to_seq meta.script |> Seq.map (opcode_action act) in
  let next : 'a t =
   fun _ ->
    match transition with
    | WaitThenJump next -> Seq.Cons (Pause, from_scene next)
    | Continuation next -> Seq.Cons (Present, from_scene next)
    | Choice c ->
        let cc = List.map (fun (c, s) -> (c, from_scene s)) c in
        Seq.Cons (Choose cc, Seq.empty)
    | EndGame -> Seq.Nil
  in
  List.to_seq [ bg; m; scr; next ] |> Seq.concat
