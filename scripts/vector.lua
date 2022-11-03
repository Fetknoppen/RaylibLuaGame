local vector = {}

vector.__index = vector

function vector.new(x,y,z)
	local t = {
		x = x or 0,
		y = y or 0,
		z = z or 0
	}
	return setmetatable(t, vector)
end

function vector.isvector(t)
	return getmetatable(t) == vector
end

function vector.__newindex(t, k , v) 
	print("vector - not possible to asign new fields")
end

function vector.__tostring(t)
	return ("(" .. t.x .. "," .. t.y .. "," .. t.z .. ")")
end

function vector:length()
	return math.sqrt((self.x*self.x)+(self.y*self.y)+(self.y*self.y))
end

function vector.__unm(t)
	if vector.isvector(t) then
		return vector.new(-t.x, -t.y, -t.z)
	else
		print("vector - expected a vector type")
	end
end

function vector.__add(a, b)
	if vector.isvector(a) and vector.isvector(b) then
		return vector.new(a.x + b.x, a.y + b.y, a.z + b.z)
	else
		print("vector - expected a vector type")
	end
end

function vector.__sub(a, b)
	if vector.isvector(a) and vector.isvector(b) then
		return vector.new(a.x - b.x, a.y - b.y, a.z - b.z)
	else
		print("vector - expected a vector type")
	end
end

function vector.__mul(a, b)
	if vector.isvector(a) and vector.isvector(b) then
		--vector vector mul
		return vector.new(a.x * b.x, a.y * b.y, a.z * b.z)
	elseif vector.isvector(a) and type(b)=="number"  then
		--vector number mul
		return vector.new(a.x * b, a.y * b, a.z * b)
	elseif type(a)=="number" and vector.isvector(b)  then
		--number vector mul
		return vector.new(a * b.x, a * b.y, a * b.z)
	else
		print("vector - expected (vector, number), (number, vector) or (vector, vector)")
	end
end

function vector.__div(a, b)
	if vector.isvector(a) and vector.isvector(b) then
		--vector vector div
		return vector.new(a.x / b.x, a.y / b.y, a.z / b.z)
	elseif vector.isvector(a) and type(b)=="number"  then
		--vector number div
		return vector.new(a.x / b, a.y / b, a.z / b)
	elseif type(a)=="number" and vector.isvector(b)  then
		--number vector div
		return vector.new(a / b.x, a / b.y, a / b.z)
	else
		print("vector - expected (vector, number), (number, vector) or (vector, vector)")
	end
end

function vector.__eq(a, b)
	if vector.isvector(a) or vector.isvector(b) then
		if a.x == b.x and a.y == b.y and a.z == b.z then
			return true
		else
			return false
		end
	else
		print("vector - expected a vector type")
	end
end


--return vector

return setmetatable(vector, {
	__call = function(_, ...)
		return vector.new(...)
	end
})