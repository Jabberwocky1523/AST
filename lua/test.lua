local function add(a, b)
    return a + b
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
local v1, idx1, sum = max(4, add(7, 2), 6, 2)
local v2 = max(5, 6, 1)
