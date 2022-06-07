module M (S : Specs.M) : sig
  type t

  val create : Cindel.renderer -> Oto.Types(S.D).scene -> t
  val step : t -> (Oto.Vm(S.D).action * t) option
  val background_of : t -> Cindel.texture
  val with_background : t -> S.D.background -> t
  val timer_of_in_ms : t -> int
  val with_timer : t -> Ticks.t -> t
end
