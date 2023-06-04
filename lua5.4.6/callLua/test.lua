name = "bob"
age = 20
mystr = "hello lua"
mytable = {name = "tom", id = 123456}

function add(x, y)
    return 2 * x + y
end

--print("name: ", name)
--print("age: ", age)

function f (x, y)
    return (x ^ 2 * math.sin(y)) / (1 - x)
end

print("------test.lua------")

var = 5

function f2()
    print("global var = ", var, var2)
end


t = {1, 2, ["a"] = 3, ["b"] = {["c"] = 'd'}}


function set_data(...)
	local args = {...}
	for i=1,#args do
		print(args[i])
	end
end


local array = {1,2,4,5,6,7,8,9,10}
 
function get_data()
	print("get_data")
	return array	
end