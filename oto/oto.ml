module type Domain = Types.Domain

module M (D : Domain) = struct
  include Types.M (D)
  include Vm.M (D)

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

  let speak actor line : opcode = { actor; line }
  let then_endgame meta = (meta, EndGame)
  let then_choose meta choices = (meta, Choice choices)
  let then_continue meta scene = (meta, Continuation scene)
  let then_pause_and_continue meta scene = (meta, WaitThenJump scene)
end