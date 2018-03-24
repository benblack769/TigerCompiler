/* error in recursion */
let
    function f(a:int):int =
        if a = 0 then 0
        else f()
in
    f(10)
end
