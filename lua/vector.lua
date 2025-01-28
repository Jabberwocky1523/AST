local mt = {}
function vector(x, y)
    local v = { x = x, y = y }
    setmetatable(v, mt)
    return v
end

mt.__add = function(v1, v2)
    return vector(v1.x + v2.x, v1.y + v2.y)
end
mt.__call = function(v)
    print(v.x, v.y)
end
mt.__mul = function(v1, v2)
    return vector(v1.x * v2.x, v1.y * v2.y)
end
mt.__index = function()
    return "123"
end
v1 = vector(1, 2)
v2 = vector(2, 3)
v3 = v1 * v2
v3()
