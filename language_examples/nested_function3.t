/* possible scope error */
let
    function f(x:int):int =
        let
            var b:int := 6
            function h():int =
                let var b:int := 4 in b + 3 end
        in
            b + h()
        end
in
    f(2)
end
