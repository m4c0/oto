type t

val epoch : t
val now : unit -> t
val ms_from_now : t -> int
