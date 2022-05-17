type background = string option
type music = string option
type pose = string option
type scene_name = string
type 'act opcode = { actor : 'act; pose : pose; line : string }
type 'act cast = { left : 'act list; middle : 'act list; right : 'act list }

type 'act scene_meta = {
  name : string;
  background : background;
  music : music;
  actors : unit -> 'act cast;
  script : 'act opcode list;
}

type 'act transition =
  | Continuation of 'act scene
  | WaitThenJump of 'act scene
  | Choice of 'act choice list
  | EndGame

and 'act choice = string * 'act scene
and 'act scene = unit -> 'act scene_meta * 'act transition

let scene_of_choice ((_, s) : _ choice) = s
