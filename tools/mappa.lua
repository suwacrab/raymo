local char = string.char

local function umap_new()
	local m = {}
	for i = 0,(8*8)-1 do m[i] = 0; end
	return m
end

local function umap_get(m,x,y)
	return m[x + (y*8)]; end

local function umap_set(m,x,y,d)
	m[x + (y*8)] = d; end

local function umap_str(m)
	local mapstr = {"+--------+\n"}
	for y = 0,7 do
		mapstr[#mapstr+1] = '|'
		for x = 0,7 do
			local d = umap_get(m,x,y)
			if d==0 then d=(' '):byte() end
			mapstr[#mapstr+1] = char(d)
		end
		mapstr[#mapstr+1] = '|\n'
	end
	mapstr[#mapstr+1] = "+--------+"
	return table.concat(mapstr)
end

local m = umap_new()
local str = "that's pretty bad bro"
print(#str)
for i = 1,#str do m[i-1] = str:sub(i,i):byte() end
print( umap_str(m) )

