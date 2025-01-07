local function add(a, b)
    return a + b
end
local function max(t)
    local args = t
    local val, idx
    for i = 1, #args do
        if (val == nil) or args[i] > val then
            val, idx = args[i], i
        end
    end
    return val, idx
end
local t = { 4, 5, 7 }
local v1, idx1 = max(t)
t[1] = 8
local v2 = max(t)
t[2] = 9
local v3 =max(t)
local v4 = max(t)
local v5 = max(t)