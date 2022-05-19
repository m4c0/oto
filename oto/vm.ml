module M (D : Types.Domain) = struct
  module Types = Types.M (D)

  type side = Left | Middle | Right

  type line = {
    side : side;
    actor : D.actor;
    pose : string option;
    text : string;
  }

  type action =
    | Background of string
    | Choose of (string * t) list
    | Music of string
    | Pause
    | Present
    | Speak of line

  and t = action Seq.t

  exception ActorNotFoundInCast

  let background x = Background x
  let music x = Music x
  let cmd_from_opt c o = Option.map c o |> Option.to_seq

  let side_of actor ({ left; middle; right } : Types.cast) =
    let eq a = actor == a in
    let exists = List.exists eq in
    if exists left then Left
    else if exists middle then Middle
    else if exists right then Right
    else raise ActorNotFoundInCast

  let opcode_action cast ({ actor; pose; line } : Types.opcode) =
    let side = side_of actor cast in
    Speak { side; actor; pose; text = line }

  let rec from_scene (s : Types.scene) : t =
    let meta, transition = s () in
    let bg = cmd_from_opt background meta.background in
    let m = cmd_from_opt music meta.music in
    let act = meta.actors () in
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
    List.to_seq [ bg; m; scr; next ] |> Seq.concat
end
