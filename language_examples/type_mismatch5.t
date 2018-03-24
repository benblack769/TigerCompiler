/* type mismatch ; while has nil type */
let
    var a:int := 1
    var b:int := while (a<5) do (a := a + 1)
in
    b
endfgh
