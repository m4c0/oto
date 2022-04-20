type side = Left | Middle | Right
type line = { side : side; actor : string; pose : string option; text : string }

type action =
  | Background of string
  | Cast of string list
  | Choose of string list
  | Music of string
  | Pause
  | Present
  | Speak of line

type t = action Seq.t

let background x = Background x
let cast x = Cast x
let music x = Music x
let cmd_from_opt c o = Option.map c o |> Option.to_seq

let actors_of ({ left; middle; right } : Types.cast) =
  match List.concat [ left; middle; right ] with
  | [] -> Seq.empty
  | x -> x |> cast |> Seq.return

let rec from_scene (s : Types.scene) : t =
  let meta, transition = s () in
  let bg = cmd_from_opt background meta.background in
  let m = cmd_from_opt music meta.music in
  let cast = meta.actors () |> actors_of in
  let next : t =
   fun _ ->
    match transition with
    | WaitThenJump next -> Seq.Cons (Pause, from_scene next)
    | Continuation next -> Seq.Cons (Present, from_scene next)
    | Choice _ -> Seq.Nil
    | EndGame -> Seq.Nil
  in
  List.to_seq [ bg; m; next; cast ] |> Seq.concat
