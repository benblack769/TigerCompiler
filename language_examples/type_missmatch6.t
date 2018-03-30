/* type mismatch ; while has nil type */
let
    var a:int := 1
    var b:int := if a then 1 else "hithere"
in
    b
end
