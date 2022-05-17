module Printer = Outo.Yaml (Poc.Printer)

let () = Printer.print_yaml Poc.game
