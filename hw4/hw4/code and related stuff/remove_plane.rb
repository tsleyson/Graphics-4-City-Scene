if ARGV[0].nil? then
    puts "Usage: remove_plane <file>"
else
    f = File.open(ARGV[0], "r")
    keepers = []
    f.each_line do |line|
        keepers << line if not line.match("\"plane\"")
    end
    File.open(ARGV[0] + ".removed", "w") do |file| 
        keepers.each {|line| file.write(line)}
    end
end
    
