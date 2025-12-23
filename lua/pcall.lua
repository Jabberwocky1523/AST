function div(a, b)
    return a / b
end

ok, result = pcall(div, 1, 2);
print(result)

