open Oto

type actor = Lefty | Midly | Righty

let title_meta = scene_meta ~background:"title" ~music:"romance-1" "Title"
let lefty_meta = scene_meta "Lefty Intro"
let midly_meta = scene_meta "Midly Intro" ~actors:(simple_cast ~left:Midly)
let righty_meta = scene_meta "Righty Intro"
let lefty_says = speak Lefty
let midly_says ?pose = speak ?pose Midly
let righty_says = speak Righty
let restaurant_meta = scene_meta ~background:"restaurant" ~music:"romance-1"
let the_end_meta = scene_meta ~background:"end" ~music:"game-over" "The End"
let the_end _ = then_endgame the_end_meta
let pause_then_end c m = (c, fun _ -> then_pause_and_continue m the_end)

let intro_meta =
  restaurant_meta "Intro"
    ~actors:(simple_cast ~left:Lefty ~middle:Midly ~right:Righty)
    ~script:
      [
        lefty_says "Hello! How are you doing?";
        lefty_says "Do you want to know what's happening?";
        righty_says "Yeah, you! Baby, baby!";
        midly_says "Click one option to select who you want to date";
        midly_says ~pose:"hot" "I'm as hot as an idol";
      ]

let intro _ =
  then_choose intro_meta
    [
      pause_then_end "Lefty" lefty_meta;
      pause_then_end "Midly" midly_meta;
      pause_then_end "Righty" righty_meta;
    ]

let game : _ Types.scene = fun _ -> then_continue title_meta intro

type p_actor = actor

module Printer = struct
  type actor = p_actor

  let actor_to_string : actor -> string = function
    | Lefty -> "lefty"
    | Midly -> "midly"
    | Righty -> "righty"
end
