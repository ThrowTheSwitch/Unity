#This rakefile sets you up to use IAR System's C Compiler and Simulator for your tests.
module RakefileConstants

  PROGRAM_FILES_PATH = ENV['ProgramFiles']
  
  IAR_ROOT_PATHS = [
    PROGRAM_FILES_PATH + '\IAR Systems\Embedded Workbench 5.0',
    PROGRAM_FILES_PATH + '\IAR Systems\Embedded Workbench 5.0 Kickstart',
    PROGRAM_FILES_PATH + '\IAR Systems\Embedded Workbench MSP430'
  ]
  
  IAR_PROC_FOLDER = '\430'
  
  IAR_ROOT_PATHS.each do |path|
    if File.exist?(path + IAR_PROC_FOLDER)
      IAR_ROOT = path
      break
    end
  end
  
  exit(-1) if IAR_ROOT.nil?

  C_EXTENSION = '.c'
  OBJ_EXTENSION = '.r43'
  BIN_EXTENSION = '.d43'

  UNIT_TEST_PATH = 'test'
  UNITY_PATH = 'src'
  SOURCE_PATH = 'src'
  BUILD_PATH = 'build'
  BIN_PATH = 'build'
  IAR_PATH = IAR_ROOT + '\common'
  IAR_BIN = IAR_PATH + '\bin'
  IAR_CORE_PATH = IAR_ROOT + '\430'
  IAR_CORE_BIN = IAR_CORE_PATH + '\bin'
  IAR_CORE_CONFIG = IAR_CORE_PATH + '\config'
  IAR_CORE_INCLUDE = IAR_CORE_PATH + '\inc'
  IAR_CORE_INCLUDE_DLIB = IAR_CORE_INCLUDE + '\dlib'
  IAR_CORE_LIB = IAR_CORE_PATH + '\lib'
  IAR_CORE_DLIB = IAR_CORE_LIB + '\dlib\dl430fn.r43'
  IAR_CORE_DLIB_CONFIG = IAR_CORE_LIB + '\dlib\dl430fn.h'

  SIMULATOR_PROCESSOR = IAR_CORE_BIN + '\430proc.dll'
  SIMULATOR_DRIVER = IAR_CORE_BIN + '\430sim.dll'
  SIMULATOR_PLUGIN = IAR_CORE_BIN + '\430bat.dll'
  SIMULATOR_BACKEND_DDF = IAR_CORE_CONFIG + '\MSP430F5438.ddf'
  PROCESSOR_TYPE = 'MSP430F5438'
  LINKER_CONFIG = IAR_CORE_CONFIG + '\lnk430f5438.xcl'
  LINKER_CONFIG_MULT = IAR_CORE_CONFIG + '\multiplier.xcl'
  
  UNITY_SRC = UNITY_PATH + '\unity.c'
  UNITY_HDR = UNITY_PATH + '\unity.h'
  UNITY_TEST_SRC = UNIT_TEST_PATH + '\testunity.c'
  UNITY_TEST_RUNNER_SRC = UNIT_TEST_PATH + '\testunity_Runner.c'
  UNITY_OBJ = BIN_PATH + '\unity' + OBJ_EXTENSION
  UNITY_TEST_OBJ = BIN_PATH + '\testunity' + OBJ_EXTENSION
  UNITY_TEST_RUNNER_OBJ = BIN_PATH + '\testunity_Runner' + OBJ_EXTENSION
  UNITY_TEST_EXEC = UNITY_TEST_OBJ.ext BIN_EXTENSION
  TEST_RESULTS = UNITY_TEST_OBJ.ext '.testpass'

  COMPILER = IAR_CORE_BIN + '\icc430.exe'
  LINKER = IAR_CORE_BIN + '\xlink.exe'
  SIMULATOR = IAR_BIN + '\CSpyBat.exe'

end

module RakefileHelpers
  include RakefileConstants

  def flush_output
    $stderr.flush
    $stdout.flush
  end

  def report message
    puts message
    flush_output
  end
  
#####################################
# IAR Compiler Command-Line
#####################################
# icc430.exe 
# --no_cse 
# --no_unroll 
# --no_inline 
# --no_code_motion 
# --no_tbaa 
# --debug 
# -D__MSP430F149__ 
# -DUNITY_FLOAT_SUPPORT_DISABLED
# -e 
# -Ol 
# --multiplier=16 
# --double=32 
# --diag_suppress Pa050 
# --diag_suppress Pe111 
# --dlib_config C:\Program Files\IAR Systems\Embedded Workbench MSP430\430\LIB\dl430fn.h 
# -I C:\Program Files\IAR Systems\Embedded Workbench MSP430\430\INC\ 
# -I C:\Program Files\IAR Systems\Embedded Workbench MSP430\430\INC\DLIB\ 
# C:\Documents and Settings\Greg\Desktop\MSP430\main.c 
# -o C:\Documents and Settings\Greg\Desktop\MSP430\Debug\Obj\ 
#####################################

  def compile(src, obj)
    execute "#{COMPILER} --no_cse --no_unroll --no_inline --no_code_motion --no_tbaa --debug -D__MSP430F149__ -DUNITY_FLOAT_SUPPORT_DISABLED -e -Ol --multiplier=16 --double=32 --diag_suppress Pa050 --diag_suppress Pe111 --dlib_config \"#{IAR_CORE_DLIB_CONFIG}\" -I\"#{IAR_CORE_INCLUDE}\" -I\"#{IAR_CORE_INCLUDE_DLIB}\" -I\"#{UNITY_PATH}\" -Isrc -Itest #{src} -o#{obj}"
  end

#####################################
# IAR Linker Command-Line
#####################################
# xlink.exe
# -IC:\Program Files\IAR Systems\Embedded Workbench MSP430\430\LIB\ 
# -rt 
# C:\Program Files\IAR Systems\Embedded Workbench MSP430\430\LIB\DLIB\dl430fn.r43
# -e_PrintfTiny=_Printf 
# -e_ScanfSmall=_Scanf 
# -s __program_start 
# -D_STACK_SIZE=50
# -D_DATA16_HEAP_SIZE=50 
# -D_DATA20_HEAP_SIZE=50
# -f C:\Program Files\IAR Systems\Embedded Workbench MSP430\430\config\lnk430F149.xcl 
# -f C:\Program Files\IAR Systems\Embedded Workbench MSP430\430\config\multiplier.xcl
# C:\Documents and Settings\Greg\Desktop\MSP430\Debug\Obj\main.r43 
# -o C:\Documents and Settings\Greg\Desktop\MSP430\Debug\Exe\MSP430Test.d43 
#####################################

  def link(prerequisites, executable)
    execute "\"#{LINKER}\" -rt \"#{IAR_CORE_DLIB}\" -e_PrintfTiny=_Printf -e_ScanfSmall=_Scanf -s __program_start -D_STACK_SIZE=50 -D_DATA16_HEAP_SIZE=50 -D_DATA20_HEAP_SIZE=50 -I\"#{IAR_CORE_LIB}\" -f \"#{LINKER_CONFIG}\" -f \"#{LINKER_CONFIG_MULT}\" #{prerequisites.join(' ')} -o #{executable}"
  end

  def run_test(executable)
    execute "\"#{SIMULATOR}\" --silent \"#{SIMULATOR_PROCESSOR}\" \"#{SIMULATOR_DRIVER}\" #{executable} --plugin \"#{SIMULATOR_PLUGIN}\" --backend -B --cpu #{PROCESSOR_TYPE} -p \"#{SIMULATOR_BACKEND_DDF}\" -d sim"
  end

  def write_result_file(filename, results)
    if (results.include?("OK\n"))
      output_file = filename.gsub(BIN_EXTENSION, '.testpass')
    else
      output_file = filename.gsub(BIN_EXTENSION, '.testfail')
    end
    File.open(output_file, 'w') do |f|
      f.print results
    end
  end

  private #####################

  def execute(command_string)
    report command_string
    output = `#{command_string}`
    report output
    if $?.exitstatus != 0
      raise "Command failed. (Returned #{$?.exitstatus})"
    end
    return output
  end

end
