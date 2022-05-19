module S = struct
  let width = 800
  let height = 600
end

module P = Pegasus.M (S) (Poc.Printer)

let () = P.run Poc.game
