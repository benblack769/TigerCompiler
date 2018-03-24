/* type mismatch ; let-in-end statement with nothing between in and end has nil type */
let
    var a:int := let var b:int := 10 in end
in
    a
end
