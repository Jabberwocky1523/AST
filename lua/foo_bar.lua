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
local v6 = max(t)
local v7 =max(t)
local v8 =max(t)
local v9 =max(t)
local v10 = max(t)
local v11 =max(t)
local v12 = max(t)
t[2] = 10
local v14 =max(t)
local v15 =max(t)
local v16 =max(t)
local v17 =max(t)
local v18 =max(t)
local v19 =max(t)
local v20 =max(t)
local v21 =max(t)
local v22 =max(t)
local v23 =max(t)
local v24 =max(t)
local v25 =max(t)
local v26 =max(t)
-- local v27 =2
-- local v28 =2
-- local v29 =2
-- local v30 =2
-- local v31 =2
-- local v32 =2
-- local v33 =2
-- local v34 =2
-- local v35 =2
-- local v36 =2