open Oto

let title_meta = scene_meta ~background:"title" ~music:"romance-1" "Title"
let lefty_meta = scene_meta "Lefty Intro"
let midly_meta = scene_meta "Midly Intro"
let righty_meta = scene_meta "Righty Intro"
let lefty_says = speak "lefty"
let midly_says = speak "midly"
let righty_says = speak "righty"
let midly_poses = pose "midly"
let restaurant_meta = scene_meta ~background:"restaurant" ~music:"romance-1"

let intro_meta =
  restaurant_meta "Intro"
    ~actors:(simple_cast ~left:"lefty" ~middle:"middly" ~right:"righty")
    ~script:
      [
        lefty_says "Hello! How are you doing?";
        lefty_says "Do you want to know what's happening?";
        righty_says "Yeah, you! Baby, baby!";
        midly_says "Click one option to select who you want to date";
        midly_poses "imponent";
        midly_says "I'm as hot as an idol";
      ]

let lefty _ = then_endgame lefty_meta
let midly _ = then_endgame midly_meta
let righty _ = then_endgame righty_meta

let intro _ =
  then_choose intro_meta [ ("Lefty", lefty); ("Midly", midly); ("Righty", righty) ]

let game : Types.scene = fun _ -> then_continue title_meta intro
let () = Dumper.print_yaml game
