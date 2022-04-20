type actor = string
type background = string option
type music = string option
type pose = string option
type scene_name = string
type opcode = { actor : actor; pose : pose; line : string }
type cast = { left : actor list; middle : actor list; right : actor list }

type scene_meta = {
  name : string;
  background : background;
  music : music;
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
