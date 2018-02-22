#============================================================
#  Author:   John Theofanopoulos
#  A simple parser.  Takes the output files generated during the
#  build process and extracts information relating to the tests.
#
#  Notes:
#    To capture an output file under VS builds use the following:
#      devenv [build instructions]  > Output.txt & type Output.txt
#
#    To capture an output file under Linux builds use the following:
#      make | tee Output.txt
#
#    To use this parser use the following command
#    ruby parseOutput.rb [options] [file]
#        options:  -xml  : produce a JUnit compatible XML file
#        file:  file to scan for results
#============================================================

# Parser class for handling the input file
class ParseOutput
  def initialize
    @xml_out = false
    @array_list = false
    @class_name = 0
    @test_suite = nil
    @total_tests = false
    @path_delim = nil
  end

  # Set the flag to indicate if there will be an XML output file or not
  def set_xml_output
    @xml_out = true
  end

  # If write our output to XML
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
    test_name = test_suite_name.split(@path_delim)

    # Remove the extension and extract the base_name
    base_name = test_name[test_name.size - 1].split('.')[0]

    # Return if the test suite hasn't changed
    return unless base_name.to_s != @test_suite.to_s

    @test_suite = base_name
    printf "New Test: %s\n", @test_suite
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

  # Adjusts the os specific members according to the current path style
  # (Windows or Unix based)
  def set_os_specifics(line)
    if line.include? '\\'
      # Windows X:\Y\Z
      @class_name = 1
      @path_delim = '\\'
    else
      # Unix Based /X/Y/Z
      @class_name = 0
      @path_delim = '/'
    end
  end

  # Main function used to parse the file that was captured.
  def process(file_name)
    @array_list = []

    puts 'Parsing file: ' + file_name

    test_pass = 0
    test_fail = 0
    test_ignore = 0
    puts ''
    puts '=================== RESULTS ====================='
    puts ''
    File.open(file_name).each do |line|
      # Typical test lines look like this:
      # <path>/<test_file>.c:36:test_tc1000_opsys:FAIL: Expected 1 Was 0
      # <path>/<test_file>.c:112:test_tc5004_initCanChannel:IGNORE: Not Yet Implemented
      # <path>/<test_file>.c:115:test_tc5100_initCanVoidPtrs:PASS
      #
      # where path is different on Unix vs Windows devices (Windows leads with a drive letter)
      set_os_specifics(line)
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
  ARGV.each do |arg|
    if arg == '-xml'
      parse_my_file.set_xml_output
    else
      parse_my_file.process(arg)
      break
    end
  end
end
