local function add(...)
    local args = { ... }
    local sum = 0
    for i, v in ipairs(args) do
        sum = v + sum
    end
    return sum
end
T = {}
T.add = add
print(T.add(1,2,3,4,5,6))
return T