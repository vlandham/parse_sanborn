#!/usr/bin/env ruby

TEMPLATE_BIN = File.expand_path(File.join(File.dirname(__FILE__), "template_matcher"))
TEMPLATE_OUTPUT = "template_matches/template_match*"
#MARKER = File.expand_path(File.join(File.dirname(__FILE__),"marker.png"))
MARKER = "marker.png"


if !File.exists?(TEMPLATE_BIN)
  puts "ERROR: can't find #{TEMPLATE_BIN}"
  raise 'no template_matcher'
end

map_dir = "./maps"
template_image = ARGV[0]

if !template_image
puts "ERROR: need template image"
raise 'error'
end


template_name = File.basename(template_image).split(".")[0]
system("rm -rf #{template_name}")
system("rm -rf #{File.dirname(TEMPLATE_OUTPUT)}")
system("mkdir -p #{template_name}")
system("mkdir -p #{File.dirname(TEMPLATE_OUTPUT)}")

output_filename = File.join(template_name, "#{template_name}_matches.html")
output_file = File.open(output_filename, 'w')
begin

output_file.puts <<EOL
<html>
<head>
</head>
<body>
<table>
EOL

def create_map_page(map, map_num, base_dir, template_match_num, coords)
  map_html_filename = "#{base_dir}/map_#{map_num}_#{template_match_num}.html"
  File.open(map_html_filename, 'w') do |file|
    file.puts <<EOF
<html>
<head>
</head>
<body>
  <div id="map" style="position: absolute; z-index:100;"><img src="../#{map}"/></div>
  <div id="point" style="position: absolute; left:#{coords[0]}px; top:#{coords[1]}px; z-index:5000;"><img src="../#{MARKER}"/></div>
</body>
</html>
EOF
  end
  map_html_filename
end


maps = Dir.glob(File.join(map_dir, "sanborn_*.jpg"))

puts "Looking at #{maps.size} maps"



maps = maps.sort_by {|fn| File.mtime(fn)}

maps.each do |map|
  #map = File.expand_path(map)
  map_num = File.basename(map).split("_")[-1]
  puts "#{map_num}"
  command = "#{TEMPLATE_BIN} #{map} #{template_image}"
  puts command
  coords = %x[#{command}]
  coords = coords.split("\n")
  template_matches = Dir.glob(TEMPLATE_OUTPUT)
  template_matches = template_matches.sort_by {|filename| File.mtime(filename) }
  template_matches.each_with_index do |template_match, index|
    template_match_coords = coords[index].split(",")
    template_match_num = template_match.split(".")[0].split("_")[-1]
    template_out_name = "#{template_name}/#{template_name}_#{map_num}_#{template_match_num}.png"
    system("mv #{template_match} #{template_out_name}")
    map_html_filename = create_map_page map, map_num, template_name, template_match_num, template_match_coords
    output_file.puts("<tr><td>#{template_match_coords[0]}</td><td>#{template_match_coords[1]}</td><td><img src=\"#{File.basename(template_out_name)}\"/></td><td><a href=\"#{File.basename(map_html_filename)}\">#{map}</a></td><td>#{map_num}</td></tr>")
    
  end
  
  
  
  
end
ensure
output_file.puts "</table>\n</body>\n</html>"
output_file.close
end
