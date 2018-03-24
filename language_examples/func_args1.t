/* should be an error in the params */
let
    function f(a:int, a:string, a:int):int =
        let var b:int := 6 in
            a + b end
in
    f(5,"Hello",10)
end
