-- function newCounter()
--     local count = 1
--     return function()
--         count = count + 1
--         return count
--     end
-- end

-- c1 = newCounter()
-- print(c1())
-- print(c1())
-- print(len(c2))

-- local function t(a)
--     local A = 1
--      A = A + 1
--      C = A + 1
-- end
-- A = 1
-- t(A)
-- print(A,C)

-- T = loadfile("./lua/test.lua")
-- T()
-- print(1, "\n")
-- C = add(1, 2)
-- print(C)

-- MT = { __newindex = function(tb, key, val) end, __index = function() print(2) end }
-- T = {}
-- setmetatable(T, MT)
-- T["t"] = 1
-- rawset(T, "t", 1)
-- print(rawget(T, "t"))
-- A = nil
-- TT = type(A)
-- print(TT, '\n')
-- if (TT == "String") then
--     print("S")
-- elseif (TT == "Number") then
--     print("N")
-- elseif (TT == "Long Long") then
--     print("L")
-- else
--     print("E")
-- end

Str = "123"
Len = string.len(Str)

print(string.sub(Str, 2, 4));
