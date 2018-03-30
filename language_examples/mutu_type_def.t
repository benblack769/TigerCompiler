let type a = {x: d, y: c}
    type b = d
    type d = a
    type f = array of c
    type c = {x: a, y: c, z: f, w: b}
    var x : c := nil
in
    x.x.x.y.z[3].x.y.w
end
