module Dumper = Dumper
module Types = Types
module Vm = Vm
open Types

let cast ?(left = []) ?(middle = []) ?(right = []) () : _ cast =
  { left; middle; right }

let simple_cast ?left ?middle ?right () : _ cast =
  {
    left = Option.to_list left;
    middle = Option.to_list middle;
    right = Option.to_list right;
  }

let empty_cast () = { left = []; middle = []; right = [] }

let scene_meta ?background ?music ?(actors = empty_cast) ?(script = []) name :
    _ scene_meta =
  { name; background; music; actors; script }

let speak ?pose actor line : _ opcode = { actor; pose; line }
let then_endgame meta = (meta, EndGame)
let then_choose meta choices = (meta, Choice choices)
let then_continue meta scene = (meta, Continuation scene)
let then_pause_and_continue meta scene = (meta, WaitThenJump scene)
