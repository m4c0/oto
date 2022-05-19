module type Domain = sig
  type actor
  type background
  type music
end

module M (D : Domain) = struct
  type pose = string option
  type scene_name = string
  type opcode = { actor : D.actor; pose : pose; line : string }

  type cast = {
    left : D.actor list;
    middle : D.actor list;
    right : D.actor list;
  }

  type scene_meta = {
    name : string;
    background : D.background option;
    music : D.music option;
    actors : unit -> cast;
    script : opcode list;
  }

  type transition =
    | Continuation of scene
    | WaitThenJump of scene
    | Choice of choice list
    | EndGame

  and choice = string * scene
  and scene = unit -> scene_meta * transition

  let scene_of_choice ((_, s) : choice) = s
end
