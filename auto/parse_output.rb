#============================================================
#  Author:   John Theofanopoulos
#  A simple parser.   Takes the output files generated during the build process and
# extracts information relating to the tests.
#
#  Notes:
#    To capture an output file under VS builds use the following:
#      devenv [build instructions]  > Output.txt & type Output.txt
#
#    To capture an output file under GCC/Linux builds use the following:
#      make | tee Output.txt
#
#    To use this parser use the following command
#    ruby parseOutput.rb [options] [file]
#        options:  -xml  : produce a JUnit compatible XML file
#        file      :  file to scan for results
#============================================================

class ParseOutput
  def initialize
    @xml_out = false
    @array_list = false
    @class_name = 0
    @test_suite = nil
    @total_tests = false
  end

  #   Set the flag to indicate if there will be an XML output file or not
  def set_xml_output
    @xml_out = true
  end

  #  If write our output to XML
  def write_xml_output
    output = File.open('report.xml', 'w')
    output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    @array_list.each do |item|
      output << item << "\n"
    end
    output << "</testsuite>\n"
  end

  # This function will try and determine when the suite is changed. This is
  # is the name that gets added to the classname parameter.
  def test_suite_verify(test_suite_name)

    # Split the path name
    test_name = if @class_name == 1
                  test_suite_name.split('\\') # Windows
                else
                  test_suite_name.split('/')  # Unix based
                end

    # Remove the extension and extract the base_name
    base_name = test_name[test_name.size - 1].split('.')[0]

    # Is this a new test suite?
    if base_name.to_s != @test_suite.to_s
      @test_suite = base_name
      printf "New Test: %s\n", @test_suite
    end

  end

  # Test was flagged as having passed so format the output
  def test_passed(array)
    last_item = array.length - 1
    test_name = array[last_item - 1]
    test_suite_verify(array[@class_name])
    printf "%-40s PASS\n", test_name

    return unless @xml_out

    @array_list.push '     <testcase classname="' + @test_suite + '" name="' + test_name + '"/>'
  end

  # Test was flagged as having passed so format the output.
  # This is using the Unity fixture output and not the original Unity output.
  def test_passed_unity_fixture(array)
    test_suite = array[0].sub('TEST(', '')
    test_suite = test_suite.sub(',', '')
    test_name = array[1].sub(')', '')
    test_suite_verify(array[@class_name])
    printf "%-40s PASS\n", test_name
    return unless @xml_out

    @array_list.push '     <testcase classname="' + test_suite + '" name="' + test_name + '"/>'
  end

  # Test was flagged as being ignored so format the output
  def test_ignored(array)
    last_item = array.length - 1
    test_name = array[last_item - 2]
    reason = array[last_item].chomp.lstrip
    class_name = array[@class_name]

    if test_name.start_with? 'TEST('
      array2 = test_name.split(' ')

      test_suite = array2[0].sub('TEST(', '')
      test_suite = test_suite.sub(',', '')
      class_name = test_suite

      test_name = array2[1].sub(')', '')
    end

    test_suite_verify(class_name)
    printf "%-40s IGNORED\n", test_name

    return unless @xml_out

    @array_list.push '     <testcase classname="' + @test_suite + '" name="' + test_name + '">'
    @array_list.push '            <skipped type="TEST IGNORED">' + reason + '</skipped>'
    @array_list.push '     </testcase>'
  end

  # Test was flagged as having failed  so format the line
  def test_failed(array)
    last_item = array.length - 1
    test_name = array[last_item - 2]
    reason = array[last_item].chomp.lstrip + ' at line: ' + array[last_item - 3]
    class_name = array[@class_name]

    if test_name.start_with? 'TEST('
      array2 = test_name.split(' ')

      test_suite = array2[0].sub('TEST(', '')
      test_suite = test_suite.sub(',', '')
      class_name = test_suite

      test_name = array2[1].sub(')', '')
    end

    test_suite_verify(class_name)
    printf "%-40s FAILED\n", test_name

    return unless @xml_out

    @array_list.push '     <testcase classname="' + @test_suite + '" name="' + test_name + '">'
    @array_list.push '            <failure type="ASSERT FAILED">' + reason + '</failure>'
    @array_list.push '     </testcase>'
  end

  # Figure out what OS we are running on. For now we are assuming if it's not Windows it must
  # be Unix based.
  def detect_os
    os = RUBY_PLATFORM.split('-')
    @class_name = if os.size == 2
                    if os[1] == 'mingw32'
                      1
                    else
                      0
                    end
                  else
                    0
                  end
  end

  # Main function used to parse the file that was captured.
  def process(name)
    @array_list = []

    detect_os

    puts 'Parsing file: ' + name

    test_pass = 0
    test_fail = 0
    test_ignore = 0
    puts ''
    puts '=================== RESULTS ====================='
    puts ''
    File.open(name).each do |line|
      # Typical test lines look like this:
      # <path>/<test_file>.c:36:test_tc1000_opsys:FAIL: Expected 1 Was 0
      # <path>/<test_file>.c:112:test_tc5004_initCanChannel:IGNORE: Not Yet Implemented
      # <path>/<test_file>.c:115:test_tc5100_initCanVoidPtrs:PASS
      #
      # where path is different on Unix vs Windows devices (Windows leads with a drive letter)
      line_array = line.split(':')

      # If we were able to split the line then we can look to see if any of our target words
      # were found.  Case is important.
      if (line_array.size >= 4) || (line.start_with? 'TEST(')
        # Determine if this test passed
        if line.include? ':PASS'
          test_passed(line_array)
          test_pass += 1
        elsif line.include? ':FAIL'
          test_failed(line_array)
          test_fail += 1
        elsif line.include? ':IGNORE:'
          test_ignored(line_array)
          test_ignore += 1
        elsif line.include? ':IGNORE'
          line_array.push('No reason given')
          test_ignored(line_array)
          test_ignore += 1
        elsif line.start_with? 'TEST('
          if line.include? ' PASS'
            line_array = line.split(' ')
            test_passed_unity_fixture(line_array)
            test_pass += 1
          end
        end
      end
    end
    puts ''
    puts '=================== SUMMARY ====================='
    puts ''
    puts 'Tests Passed  : ' + test_pass.to_s
    puts 'Tests Failed  : ' + test_fail.to_s
    puts 'Tests Ignored : ' + test_ignore.to_s
    @total_tests = test_pass + test_fail + test_ignore

    return unless @xml_out

    heading = '<testsuite name="Unity" tests="' + @total_tests.to_s + '" failures="' + test_fail.to_s + '"' + ' skips="' + test_ignore.to_s + '">'
    @array_list.insert(0, heading)
    write_xml_output
  end
end

# If the command line has no values in, used a default value of Output.txt
parse_my_file = ParseOutput.new

if ARGV.size >= 1
  ARGV.each do |a|
    if a == '-xml'
      parse_my_file.set_xml_output
    else
      parse_my_file.process(a)
      break
    end
  end
end
