#!/usr/bin/ruby
# PURPOSE: This is a similar implementation to functs.pl in ruby.
#	protos.rb does what I always wanted functs.pl to do.  It
#	Generates a header file and if the comment
# /*
#  * function
#  * prototypes
#  */
#  	is in the code it will update all the static prototypes
#  	and write the updated file to fname.new.
class Protos
  def initialize(fn)
    @fname=fn
    @global=Array.new
    @local=Array.new
    prevline=String.new

    File.open(fn) do |i|
      while line = i.gets
	# Function declaration will have a letter in the first column
	# and not have a semicolon at the end of the line.
	if line =~ /^[a-zA-Z].*\(.*[^;]/
	  line[/^.* (\b.*)\(.*/]
	  func = $1
	  #Remove possible inline funct body.ie int f(int i){return i;}
	  line.sub!(/\s*\{.*/, '')
	  # Is this a function prototype? Toss it.
	  if line =~ /^[a-zA-Z].*;\s*/
	    #puts "Found function prototype, tossing #{line}"
	    next
	  # Is the retval on $prevline?
	  elsif line !~ /^\w*\s+\w.*\s*\(/
	    line=prevline.chomp + " " + line
	  # Is funct definition continued on the next?
	  elsif line !~ /\)/
	    line.sub!(/\s*$/, '')
	    while ln = i.gets
	      if ln =~ /\)/
		ln[/\s*(.*\))/]
		line=line + " #{$1}"
		break
	      else
		ln[/\s*(.*)\s*/]
		line=line + " #{$1}"
	      end
	    end
	  end
	  if line =~ /static/
	    @local.push "#{line.chomp};"
	  else
	    @global.push "#{line.chomp};"
	  end
	  #puts "Line:#{line}"
	end
	prevline=line
      end
    end
    #puts "Read in #{@global.size} globals and #{@local.size} locals"
  end
  def write_local(fn)
    if fn == @fname
      puts "Output filename must differ from #{@fname}."
      return
    end
    File.open(@fname) do |i|
    File.open(fn, "w") do |o|
      while line = i.gets
        if line.chomp! == " * prototypes"
	  o.puts line
	  o.puts " */"
	  o.puts @local
          while line = i.gets
	    break	if line !~ /static| *\//
	  end
	end
	o.puts line
      end
    end
    end
  end
  def write_global(fn)
    d=Time.now
    File.open(fn, "w") do |out|
      name=fn.sub(/\./, '_')
      name.tr!('a-z', 'A-Z')
      out.puts "/* #{fn}			DATE: #{d.strftime("%Y-%m-%d")}"
      out.puts " */"
      out.puts "#ifndef __#{name}__"
      out.puts "#define __#{name}__"
      out.puts ""
      out.puts "/* Global count = #{@global.size} */"
      out.puts @global
      out.puts "#endif /* __#{name}__ */"
    end
  end
end

#
# Script
# Body
#
src=$*[0]
hdr=src.sub(/\.c/, '.h')

proto = Protos.new(src)
proto.write_local("#{src}.new")
proto.write_global(hdr)
