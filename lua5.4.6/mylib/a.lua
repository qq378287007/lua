--[[ 这里"require"的参数对应C库中"luaopen_libmylib()"中的"libmylib"。
     C库就放在"a.lua"的同级目录，"require"可以找到。
]]

local libmylib = require("libmylib")

-- 结果与上面的例子中相同，但是这里是通过调用C库中的函数实现。
print(libmylib.mysin(3.14 / 2))    --> 0.99999968293183


local a, b = 3.14, 1.57

print(libmylib.add(a, b), libmylib.sub(a, b))   -- 4.71. 1.57

print(libmylib.avg())  -- 0.0
 
print(libmylib.avg(1,2,3,4,5)) -- 3.0
 
print(libmylib.fn(true), libmylib.fn(10), libmylib.fn("abc")) -- BOOLEAN NUMBER STRING
