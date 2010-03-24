
require "#{File.expand_path(File.dirname(__FILE__))}/colour_prompt"

$colour_output = true

def report(message)
  if not $colour_output
    $stdout.puts(message)
  else
    message.each_line do |line|
      line.chomp!
      colour = case(line)
        when /Tests\s+(\d+)\s+Failures\s+\d+\s+Ignored/
          ($1.to_i == 0) ? :green : :red
        when /PASS/
          :green
        when /^OK$/
          :green
        when /(?:FAIL|ERROR)/
          :red
        when /IGNORE/
          :yellow
        when /^(?:Creating|Compiling|Linking)/
          :white
        else
          :blue
      end
      colour_puts(colour, line)
    end
  end
  $stdout.flush
  $stderr.flush
end