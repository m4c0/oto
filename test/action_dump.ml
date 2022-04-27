let chooser l = List.tl l |> List.hd
let () = Oto.Dumper.print_actions chooser Poc.game
