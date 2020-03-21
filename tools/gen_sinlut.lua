local pi = math.pi
local sin,cos = math.sin,math.cos

local gen_sinlut = function(name,len,shift)
	local lutstr = { ("const unsigned short %s[0x%04X] =\n{\n\t"):format(name,len) }
	for i = 0,len-1 do
		local n = sin( pi*i*2/len ) * (1<<shift)
		n = math.floor(n) & 0xFFFF
		lutstr[#lutstr+1] = ("0x%04X,"):format(n)
		if ((i+1)%8)==0 then lutstr[#lutstr+1] = "\n\t" end
	end
	
	lutstr[#lutstr+1] = "\n};\n"
	return table.concat(lutstr)
end

local save_sinlut = function()
	local lut,name,len,shift
	if arg[1] then
		name = arg[1]
		len = arg[2]
		shift = arg[3]
	else
		print("enter the lut's name"); name = io.read()
		print("enter the lut's size"); len = tonumber(io.read())
		print("enter the bit shift"); shift = tonumber(io.read())
	end	
	lut = gen_sinlut(name,len,shift)
	print(lut)
end

save_sinlut()

