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

-- Str = "123"
-- Len = string.len(Str)

-- print(string.sub(Str, 2, 4));

-- local T = { c = 1 }
-- function T:add(a, b)
--     return a + b + self.c
-- end

-- C = T:add(1, 2)
-- print(C)

-- A = "aaa"
-- D = A.format("{} {} %d", 1.23232, 1)
-- B = A:upper()
-- C = A.sub(A, 1, 3):upper():lower()
-- ST, En = A:find("c")
-- E = A:gsub("c", "c")
-- print(E)

-- T = { 1, 2, 3 }
-- S = table.concat(T, "||")
-- TT = table.pack(1, 2, "concat")
-- T1, T2, T3 = table.unpack(TT)
-- print(T1, T2, T3)

-- T = math.max(1, 2, 3, "33.33")
-- print(T)

i = io.open("./lua/test.ast")
a = i:read()
print(a)
b = io.read()
print(b)
