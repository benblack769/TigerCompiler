/* lets can be used in binary ops */
let
    var a:int := 5 + (let var c:int := 3 in c end)
in
    a
end
