type background = string option
type music = string option
type actor = string
type scene_name = string
type opcode = Speech of actor * string | Pose of actor * string
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
  | Choice of (string * scene) list
  | EndGame

and scene = unit -> scene_meta * transition
