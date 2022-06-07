module M (S : Specs.M) : sig
  type t

  val create : Cindel.renderer -> Oto.Types(S.D).scene -> t
  val step : t -> (Oto.Vm(S.D).action * t) option
  val background_of : t -> Cindel.texture
  val with_background : t -> S.D.background -> t
  val timer_of : t -> Ticks.t
  val with_timer : t -> Ticks.t -> t
end
