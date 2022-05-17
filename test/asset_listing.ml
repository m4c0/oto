module Printer = Outo.Assets (Poc.Printer)

let () = Printer.assets_of_scene Poc.game |> List.iter print_endline
