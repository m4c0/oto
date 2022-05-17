let chooser l = List.tl l |> List.hd
let () = Poc.Printer.print_actions chooser Poc.game
