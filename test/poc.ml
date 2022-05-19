module Printer = struct
  type actor = Lefty | Midly | MidlyHot | Righty
  type background = Title | Restaurant | End
  type music = Romance | GameOver
  type side = Left | Middle | Right

  let actor_to_string : actor -> string = function
    | Lefty -> "lefty"
    | Midly -> "midly"
    | MidlyHot -> "midly-hot"
    | Righty -> "righty"

  let background_to_string : background -> string = function
    | Title -> "title"
    | Restaurant -> "restaurant"
    | End -> "end"

  let music_to_string : music -> string = function
    | Romance -> "romance-1"
    | GameOver -> "game-over"

  let side_to_string : side -> string = function
    | Left -> "left"
    | Middle -> "middle"
    | Right -> "right"
end

open Oto.M (Printer)

let lefty_says = speak Lefty
let midly_says = speak Midly
let midly_hot_says = speak MidlyHot
let righty_says = speak Righty
let title_meta = scene_meta ~background:Title ~music:Romance "Title"
let lefty_meta = scene_meta "Lefty Intro"

let midly_meta =
  scene_meta "Midly Intro"
    ~cast:(fun _ -> Some Left)
    ~script:[ midly_says "This is me" ]

let righty_meta = scene_meta "Righty Intro"
let restaurant_meta = scene_meta ~background:Restaurant ~music:Romance
let the_end_meta = scene_meta ~background:End ~music:GameOver "The End"
let the_end _ = then_endgame the_end_meta
let pause_then_end c m = (c, fun _ -> then_pause_and_continue m the_end)

let intro_meta =
  restaurant_meta "Intro"
    ~cast:(function
      | Lefty -> Some Left
      | Midly | MidlyHot -> Some Middle
      | Righty -> Some Right)
    ~script:
      [
        lefty_says "Hello! How are you doing?";
        lefty_says "Do you want to know what's happening?";
        righty_says "Yeah, you! Baby, baby!";
        midly_says "Click one option to select who you want to date";
        midly_hot_says "I'm as hot as an idol";
      ]

let intro _ =
  then_choose intro_meta
    [
      pause_then_end "Lefty" lefty_meta;
      pause_then_end "Midly" midly_meta;
      pause_then_end "Righty" righty_meta;
    ]

let game : scene = fun _ -> then_continue title_meta intro
