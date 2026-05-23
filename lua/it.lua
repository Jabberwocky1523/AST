local i = 0
::test::
print(i)
i = i + 1
if i == 2 then
    return
else
    goto test
end
