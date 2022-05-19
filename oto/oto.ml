module type Domain = Types.Domain

module M (D : Domain) = struct
  include Types.M (D)
  include Vm.M (D)

  let empty_cast _ = None

  let scene_meta ?background ?music ?(cast = empty_cast) ?(script = []) name :
      scene_meta =
    { name; background; music; cast; script }

  let speak actor line : opcode = { actor; line }
  let then_endgame meta = (meta, EndGame)
  let then_choose meta choices = (meta, Choice choices)
  let then_continue meta scene = (meta, Continuation scene)
  let then_pause_and_continue meta scene = (meta, WaitThenJump scene)
end
