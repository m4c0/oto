let title_meta = Oto.scene_meta ~background:"title" ~music:"romance-1" ()
let lefty_meta = Oto.scene_meta ()
let midly_meta = Oto.scene_meta ()
let righty_meta = Oto.scene_meta ()
let lefty_says = Oto.speak "lefty"
let midly_says = Oto.speak "midly"
let righty_says = Oto.speak "righty"
let midly_poses = Oto.pose "midly"
let restaurant_meta = Oto.scene_meta ~background:"restaurant" ~music:"romance-1"

let intro_meta =
  restaurant_meta ()
    ~actors:(Oto.simple_cast ~left:"lefty" ~middle:"middly" ~right:"righty")
    ~script:
      [
        lefty_says "Hello! How are you doing?";
        lefty_says "Do you want to know what's happening?";
        righty_says "Yeah, you! Baby, baby!";
        midly_says "Click one option to select who you want to date";
        midly_poses "imponent";
        midly_says "I'm as hot as an idol";
      ]

let lefty _ = (lefty_meta, Oto.EndGame)
let midly _ = (midly_meta, Oto.EndGame)
let righty _ = (righty_meta, Oto.EndGame)

let intro _ =
  ( intro_meta,
    Oto.Choice [ ("Lefty", lefty); ("Midly", midly); ("Righty", righty) ] )

let game : Oto.scene = fun _ -> (title_meta, Oto.Continuation intro)