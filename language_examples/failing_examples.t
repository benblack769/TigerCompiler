let
  var  table := "arg"
  function value(parameter : string) =
    parameter := "Tiger is the best language\n"
in
  table[0].val := 51
  /* Now table[1].val = 51. */
end
