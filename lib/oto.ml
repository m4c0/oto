type background = string option
type music = string option
type actor = string
type scene_name = string
type opcode = Speech of actor * string | Pose of actor * string
type choice = scene_name * string
type outcome = Skip of scene_name | WaitThenJump of scene_name | Choice of choice list

type cast = {
  left : actor list;
  middle : actor list;
  right : actor list;
}

type scene = {
  name : scene_name;
  background : background;
  music : music;
  actors : cast;
  script : opcode list;
}
