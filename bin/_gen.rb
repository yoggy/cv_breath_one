#!/usr/bin/ruby

require 'pp'

path = "/path/to/movies/*.mp4"

files = Dir.glob(path).sort

files.each do |f|
  basename = File.basename(f, ".mp4")
  dirname  =  File.dirname(f)

  img_filename = dirname + "/" + basename + ".jpg"

  cmd = "cv_breath_one.exe #{f} 0 -1 5"
  puts cmd

  system cmd

end
