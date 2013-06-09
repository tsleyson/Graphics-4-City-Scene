f = File.open("car_points.txt", "r")
ind = 0
length = 0
f.each_line do |line|
	if line.match("\/\/ ") then
		ind = ind + 1
		if line.match("\/\/ TEX_TIRE_FRONT_BACK") then
			puts "#{ind},"
		end
    elsif line.match("\{\{") then
        length += 1
    end
end
puts "#{length}"
