function newCounter()
    local count = 1
    return function()
        count = count + 1
        return count
    end
end

c1 = newCounter()
print(c1())
print(c1())
