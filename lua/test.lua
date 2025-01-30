
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
    return val, idx, add(1, 2)
end

local v1, idx1, sum = max(9, 10)
t = { 1, 2, 3, 4, 5 }
print(t)
