/* possible scope error */
let
    function f(x:int):int =
        let
            function h():int =
                let var b:int := 4 in b + 3 end
        in
            b + h()
        end
in
    f(2)
end
