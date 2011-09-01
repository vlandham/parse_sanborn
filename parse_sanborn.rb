#!/usr/bin/env ruby

require 'net/http'
require 'uri'
#require 'mechanize'

require "open-uri"
#require 'yaml'
#require 'timeout'

MAX_WIDTH = 6000.0
MAX_HEIGHT = 7117.0
base_url = 'http://kchistory.org/cgi-bin/getimage.exe'
zoom_level = 0.50

width = (MAX_WIDTH * zoom_level) + 500.0
height = (MAX_HEIGHT * zoom_level) + 500.0
zoom = zoom_level * 100.0
root = "/Sanborn"

params = { "CISOROOT" => root,
           "DMSCALE" => zoom,
           "DMWIDTH" => width,
           "DMHEIGHT" => height,
           "CISOPTR" => "1"
}

#possible_ids = [1..1000]
possible_ids = (2001..3000).to_a

possible_ids.each do |possible_id|
  params["CISOPTR"] = possible_id.to_s
  param_string = params.to_a.map {|key, value| "#{key}=#{value}"}.join("&")
  full_url = base_url + "?" + param_string
  image_path = File.join(File.dirname(__FILE__), "sanborn_kc_#{possible_id}.jpg")

  puts full_url
  puts image_path
  url = URI.parse(full_url)
  #response = Net::HTTP.get_response(url)
  #puts response.inspect

  #Net::HTTP.start(url.host, url.port) do |http|
  #  resp, data = http.get(url.path)
  #  puts resp.body.inspect
  #  open(image_path, "wb" ) { |file| file.write(resp.body) }
 # end

  content = open(full_url).read
  unless content =~ /^[Ee]rror/
    File.open(image_path, 'wb') do |fo|
      fo.write content
    end
  end

end

