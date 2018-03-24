/* bad scope example, var j used before declared or defined */
let
    function f(x:int):int =
        let
            var b := 5 + j
            function h():int =
                let var j:int := 2 in j end 
        in
            b + h()
        end
in
    f(6)
end
