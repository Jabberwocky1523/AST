local function add(a, b)
    return a + b, 2
end
local function max(t)
    local args = t
    local val, idx
    for i = 1, #args do
        if (val == nil) or args[i] > val then
            val, idx = args[i], i
        end
    end
    -- local sum = add(1, 2)
    return val, idx, add(1, 2), add(3, 5)
end
local t = { 4, 5, add(5, 5), add(7, 5), 99 }
local v1, idx1, sum, sum2, te = max(t)
print(v1, idx1, sum, sum2, te)
