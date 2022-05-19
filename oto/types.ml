module type Domain = sig
  type actor
  type background
  type music
  type side
end

module M (D : Domain) = struct
  type cast = D.actor -> D.side option
  type opcode = { actor : D.actor; line : string }
  type scene_name = string

  type scene_meta = {
    name : string;
    background : D.background option;
    music : D.music option;
    cast : cast;
    script : opcode list;
  }

  type transition =
    | Continuation of scene
    | WaitThenJump of scene
    | Choice of choice list
    | EndGame

  and choice = string * scene
  and scene = unit -> scene_meta * transition

  let actor_of_opcode ({ actor; _ } : opcode) = actor
  let scene_of_choice ((_, s) : choice) = s
end
