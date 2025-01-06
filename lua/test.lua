local function add(a, b)
    return a + b
end
local function assert(v)
    if not v then fail() end
end
local function max(...)
    local args = { ... }
    local val, idx
    for i = 1, #args do
        if (val == nil) or args[i] > val then
            val, idx = args[i], i
        end
    end
    return val, idx, 2
end
local v1, idx1, sum = max(4, 6, 2, add(3, 4))
assert(v1 == 7)
local v2 = max(5, 6, 1)
