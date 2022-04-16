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

let cast ?(left = []) ?(middle = []) ?(right = []) () : cast =
  { left; middle; right }

let simple_cast ?left ?middle ?right () : cast =
  {
    left = Option.to_list left;
    middle = Option.to_list middle;
    right = Option.to_list right;
  }

let empty_cast () = { left = []; middle = []; right = [] }

let scene_meta ?background ?music ?(actors = empty_cast) ?(script = []) name :
    scene_meta =
      { name; background; music; actors; script }

let speak actor line = Speech (actor, line)
let pose actor id = Pose (actor, id)

type transition =
  | Continuation of scene
  | WaitThenJump of scene
  | Choice of (string * scene) list
  | EndGame
and scene = unit -> scene_meta * transition
