module M (D : Types.Domain) = struct
  module Types = Types.M (D)

  type line = { side : D.side; actor : D.actor; text : string }

  type action =
    | Background of D.background
    | Choose of (string * t) list
    | Music of D.music
    | Pause
    | Present
    | Speak of line

  and t = action Seq.t

  exception ActorNotFoundInCast

  let background x = Background x
  let music x = Music x
  let cmd_from_opt c o = Option.map c o |> Option.to_seq

  let opcode_action cast ({ actor; line } : Types.opcode) =
    let side =
      match cast actor with Some x -> x | None -> raise ActorNotFoundInCast
    in
    Speak { side; actor; text = line }

  let rec from_scene (s : Types.scene) : t =
    let meta, transition = s () in
    let bg = cmd_from_opt background meta.background in
    let m = cmd_from_opt music meta.music in
    let scr = List.to_seq meta.script |> Seq.map (opcode_action meta.cast) in
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
