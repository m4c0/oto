module type Domain = Types.Domain

module M (D : Domain) = struct
  include Types.M (D)
  include Vm.M (D)
  include Builder.M (D)
end
