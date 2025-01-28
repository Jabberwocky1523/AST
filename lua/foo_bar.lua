local function add(a, b)
    return a + b
end
-- local function max(t)
--     local args = t
--     local val, idx
--     for i = 1, #args do
--         if (val == nil) or args[i] > val then
--             val, idx = args[i], i
--         end
--     end
--     local sum = add(1, 2)
--     return val, idx, add(1, 2)
-- end
local b = add(1, add(1, 1))
-- local t = { 4, 5, add(5, 5), add(7, 5), 7 }
-- local v1, idx1, sum = max(t)
print(v1, b)
