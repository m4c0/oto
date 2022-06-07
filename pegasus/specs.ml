module type M = sig
  module D : Oto.Domain

  val width : int
  val height : int
  val load_background : D.background -> Cindel.surface
  val load_music : D.music -> Cindel.audio_callback
end
