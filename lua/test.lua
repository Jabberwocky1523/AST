local function add(a, b)
    return a + b
end
local function max(a, b, ...)
    local args = { ... }
    local val, idx
    for i = 1, #args do
        if (val == nil) or args[i] > val then
            val, idx = args[i], i
        end
    end
    return val, idx, add(1, 2)
end

local v1, idx1, sum = max(4, 6)
-- local v2 = max(5, 6, 1)
print(v1, idx1, sum)
-- function f(...)
--     local args = { ... }
--     for i = 1, #args do
--         print(args[i])
--     end
-- end

-- f(1, 2, 3, 4)
