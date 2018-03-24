/* "this should pass" */
let
    function f(x : int) : int = 
        if x = 0 then 2 
        else h(x-1) + 2
    function h(y : int) : int = 
        if y = 0 then 1 
        else f(y-1) + 1
in
    f(10)
end
