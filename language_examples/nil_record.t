let
  type rec = { val : int }
  var table : rec := nil
in
  table.val = 2
  /* Now table[1].val = 51. */
end
