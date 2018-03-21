let
  type rec = { val : int }
  type rec_arr = array of rec
  var  table := rec_arr[2] of nil
in
  for i := 0 to 1 do
    table.t[i] := rec { val = 42 };
  table.t[0].val := 51
  /* table[1].val = 42. */
end
