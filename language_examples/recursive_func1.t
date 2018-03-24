/* error in recursive call */
let
    function f(a:int):int =
        if a = 0 then 0
        else f("hello")
in
    f(10)
end
