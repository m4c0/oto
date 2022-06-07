module M (S : Specs.M) = struct
  module Vm = Oto.Vm (S.D)

  type t = {
    background : Cindel.texture;
    renderer : Cindel.renderer;
    timer : Ticks.t;
    vm : Vm.t;
  }

  let create renderer s =
    {
      background = Cindel.create_empty_texture renderer;
      renderer;
      timer = Ticks.now ();
      vm = Vm.from_scene s;
    }

  let step x =
    match x.vm () with
    | Cons (act, vm) -> Some (act, { x with vm })
    | Nil -> None

  let background_of x = x.background

  let with_background x b =
    {
      x with
      background = S.load_background b |> Cindel.create_texture x.renderer;
    }

  let timer_of x = x.timer
  let with_timer x timer = { x with timer }
end
