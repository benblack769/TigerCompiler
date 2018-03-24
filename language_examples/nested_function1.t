/* "this should work" */
let
    function f(x:int):int =
        var y := 10
        function h():int = 
            var j := y + 5 /* nested funcs should have access to outer func's local vars */
        x = y + h()
in
    f(10)
end
