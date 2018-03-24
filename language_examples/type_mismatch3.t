/* error in types ; assignment may not be used in expression */
let
    var a:int := 5 + (let var c:int := 3 in c end)
in
    a
end
