type side = Left | Middle | Right
type line = { side : side; actor : string; pose : string option; text : string }

type action =
  | Background of string
  | Cast of string list
  | Choose of (string * t) list
  | Music of string
  | Pause
  | Present
  | Speak of line

and t = action Seq.t

let background x = Background x
let cast x = Cast x
let music x = Music x
let cmd_from_opt c o = Option.map c o |> Option.to_seq

let all_cast_of ({ left; middle; right } : Types.cast) =
  match List.concat [ left; middle; right ] with
  | [] -> Seq.empty
  | x -> x |> cast |> Seq.return

let side_of actor ({ left; middle; right } : Types.cast) =
  if List.exists (String.equal actor) left then Left
  else if List.exists (String.equal actor) middle then Middle
  else if List.exists (String.equal actor) right then Right
  else "Actor not found in cast: " ^ actor |> failwith

let opcode_action cast ({ actor; pose; line } : Types.opcode) =
  let side = side_of actor cast in
  Speak { side; actor; pose; text = line }

let rec from_scene (s : Types.scene) : t =
  let meta, transition = s () in
  let bg = cmd_from_opt background meta.background in
  let m = cmd_from_opt music meta.music in
  let act = meta.actors () in
  let cast = all_cast_of act in
  let scr = List.to_seq meta.script |> Seq.map (opcode_action act) in
  let next : t =
   fun _ ->
    match transition with
    | WaitThenJump next -> Seq.Cons (Pause, from_scene next)
    | Continuation next -> Seq.Cons (Present, from_scene next)
    | Choice c ->
        let cc = List.map (fun (c, s) -> (c, from_scene s)) c in
        Seq.Cons (Choose cc, Seq.empty)
    | EndGame -> Seq.Nil
  in
  List.to_seq [ bg; m; cast; scr; next ] |> Seq.concat
