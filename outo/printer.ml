module type M = sig
  include Oto.Domain

  val actor_to_string : actor -> string
  val background_to_string : background -> string
  val music_to_string : music -> string
end
