let type a = {x: int, y: int}
    type c = a
    var i : a := a {x = 2, y = 3}
    var j : c := c {x = 2, y = 3}
in
    i := j
end
