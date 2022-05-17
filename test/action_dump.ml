module Printer = Outo.Action (Poc.Printer)

let chooser l = List.tl l |> List.hd
let () = Printer.print_actions chooser Poc.game
