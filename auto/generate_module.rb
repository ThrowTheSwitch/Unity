# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ==========================================

# This script creates all the files with start code necessary for a new module.
# A simple module only requires a source file, header file, and test file.
# Triad modules require a source, header, and test file for each triad type (like model, conductor, and hardware).

require 'rubygems'
require 'fileutils'

#TEMPLATE_TST
TEMPLATE_TST ||= %q[#include "unity.h"
%2$s#include "%1$s.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_%1$s_NeedToImplement(void)
{
    TEST_IGNORE_MESSAGE("Need to Implement %1$s");
}
]

#TEMPLATE_SRC
TEMPLATE_SRC ||= %q[%2$s#include "%1$s.h"
]

#TEMPLATE_INC
TEMPLATE_INC ||= %q[#ifndef _%3$s_H
#define _%3$s_H%2$s

#endif // _%3$s_H
]

class UnityModuleGenerator

  ############################
  def initialize(options=nil)

    here = File.expand_path(File.dirname(__FILE__)) + '/'

    @options = UnityModuleGenerator.default_options
    case(options)
      when NilClass then @options
      when String   then @options.merge!(UnityModuleGenerator.grab_config(options))
      when Hash     then @options.merge!(options)
      else          raise "If you specify arguments, it should be a filename or a hash of options"
    end

    # Create default file paths if none were provided
    @options[:path_src] = here + "../src/"    if @options[:path_src].nil?
    @options[:path_inc] = @options[:path_src] if @options[:path_inc].nil?
    @options[:path_tst] = here + "../test/"   if @options[:path_tst].nil?
    @options[:path_src] += '/'                unless (@options[:path_src][-1] == 47)
    @options[:path_inc] += '/'                unless (@options[:path_inc][-1] == 47)
    @options[:path_tst] += '/'                unless (@options[:path_tst][-1] == 47)

    #Built in patterns
    @patterns = { 'src' => {''         => { :inc => [] } },
                  'dh'  => {'Driver'   => { :inc => ['%1$sHardware.h'] },
                            'Hardware' => { :inc => [] }
                           },
                  'dih' => {'Driver'   => { :inc => ['%1$sHardware.h', '%1$sInterrupt.h'] },
                            'Interrupt'=> { :inc => ['%1$sHardware.h'] },
                            'Hardware' => { :inc => [] }
                           },
                  'mch' => {'Model'    => { :inc => [] },
                            'Conductor'=> { :inc => ['%1$sModel.h', '%1$sHardware.h'] },
                            'Hardware' => { :inc => [] }
                           },
                  'mvp' => {'Model'    => { :inc => [] },
                            'Presenter'=> { :inc => ['%1$sModel.h', '%1$sView.h'] },
                            'View'     => { :inc => [] }
                           }
                }
  end

  ############################
  def self.default_options
    {
      :pattern         => "src",
      :includes        =>
      {
          :src         => [],
          :inc         => [],
          :tst         => [],
      },
      :update_svn      => false,
      :boilerplates    => {},
      :test_prefix     => 'Test',
    }
  end

  ############################
  def self.grab_config(config_file)
    options = self.default_options
    unless (config_file.nil? or config_file.empty?)
      require 'yaml'
      yaml_guts = YAML.load_file(config_file)
      options.merge!(yaml_guts[:unity] || yaml_guts[:cmock])
      raise "No :unity or :cmock section found in #{config_file}" unless options
    end
    return(options)
  end

  ############################
  def files_to_operate_on(module_name, pattern=nil)
    #create triad definition
    prefix = @options[:test_prefix] || 'Test'
    triad = [ { :ext => '.c', :path => @options[:path_src],        :template => TEMPLATE_SRC, :inc => :src, :boilerplate => @options[:boilerplates][:src] },
              { :ext => '.h', :path => @options[:path_inc],        :template => TEMPLATE_INC, :inc => :inc, :boilerplate => @options[:boilerplates][:inc] },
              { :ext => '.c', :path => @options[:path_tst]+prefix, :template => TEMPLATE_TST, :inc => :tst, :boilerplate => @options[:boilerplates][:tst] },
            ]

    #prepare the pattern for use
    patterns = @patterns[(pattern || @options[:pattern] || 'src').downcase]
    raise "ERROR: The design pattern '#{pattern}' specified isn't one that I recognize!" if patterns.nil?

    # Assemble the path/names of the files we need to work with.
    files = []
    triad.each do |triad|
      patterns.each_pair do |pattern_file, pattern_traits|
        files << {
          :path => "#{triad[:path]}#{module_name}#{pattern_file}#{triad[:ext]}",
          :name => "#{module_name}#{pattern_file}",
          :template => triad[:template],
          :boilerplate => triad[:boilerplate],
          :includes => case(triad[:inc])
                         when :src then @options[:includes][:src] | pattern_traits[:inc].map{|f| f % [module_name]}
                         when :inc then @options[:includes][:inc]
                         when :tst then @options[:includes][:tst] | pattern_traits[:inc].map{|f| "Mock#{f}"% [module_name]}
                       end
        }
      end
    end

    return files
  end

  ############################
  def generate(module_name, pattern=nil)

    files = files_to_operate_on(module_name, pattern)

    #Abort if any module already exists
    files.each do |file|
      raise "ERROR: File #{file[:name]} already exists. Exiting." if File.exist?(file[:path])
    end

    # Create Source Modules
    files.each_with_index do |file, i|
      File.open(file[:path], 'w') do |f|
        f.write(file[:boilerplate] % [file[:name]]) unless file[:boilerplate].nil?
        f.write(file[:template] % [ file[:name],
                                    file[:includes].map{|f| "#include \"#{f}\"\n"}.join,
                                    file[:name].upcase ]
               )
      end
      if (@options[:update_svn])
        `svn add \"#{file[:path]}\"`
        if $?.exitstatus == 0
          puts "File #{file[:path]} created and added to source control"
        else
          puts "File #{file[:path]} created but FAILED adding to source control!"
        end
      else
        puts "File #{file[:path]} created"
      end
    end
    puts 'Generate Complete'
  end

  ############################
  def destroy(module_name, pattern=nil)

    files_to_operate_on(module_name, pattern).each do |filespec|
      file = filespec[:path]
      if File.exist?(file)
        if @options[:update_svn]
          `svn delete \"#{file}\" --force`
          puts "File #{file} deleted and removed from source control"
        else
          FileUtils.remove(file)
          puts "File #{file} deleted"
        end
      else
        puts "File #{file} does not exist so cannot be removed."
      end
    end
    puts "Destroy Complete"
  end

end

############################
#Handle As Command Line If Called That Way
if ($0 == __FILE__)
  destroy = false
  options = { }
  module_name = nil

  # Parse the command line parameters.
  ARGV.each do |arg|
    case(arg)
      when /^-d/      then destroy = true
      when /^-u/      then options[:update_svn] = true
      when /^-p(\w+)/ then options[:pattern] = $1
      when /^-s(.+)/  then options[:path_src] = $1
      when /^-i(.+)/  then options[:path_inc] = $1
      when /^-t(.+)/  then options[:path_tst] = $1
      when /^-y(.+)/  then options = UnityModuleGenerator.grab_config($1)
      when /^(\w+)/
        raise "ERROR: You can't have more than one Module name specified!" unless module_name.nil?
        module_name = arg
      when /^-(h|-help)/
        ARGV = []
      else
        raise "ERROR: Unknown option specified '#{arg}'"
    end
  end

  if (!ARGV[0])
    puts [ "\nGENERATE MODULE\n-------- ------",
           "\nUsage: ruby generate_module [options] module_name",
           "  -i\"include\" sets the path to output headers to 'include' (DEFAULT ../src)",
           "  -s\"../src\"  sets the path to output source to '../src'   (DEFAULT ../src)",
           "  -t\"C:/test\" sets the path to output source to 'C:/test'  (DEFAULT ../test)",
           "  -p\"MCH\"     sets the output pattern to MCH.",
           "              dh  - driver hardware.",
           "              dih - driver interrupt hardware.",
           "              mch - model conductor hardware.",
           "              mvp - model view presenter.",
           "              src - just a single source module. (DEFAULT)",
           "  -d          destroy module instead of creating it.",
           "  -u          update subversion too (requires subversion command line)",
           "  -y\"my.yml\"  selects a different yaml config file for module generation",
           "" ].join("\n")
    exit
  end

  raise "ERROR: You must have a Module name specified! (use option -h for help)" if module_name.nil?
  if (destroy)
    UnityModuleGenerator.new(options).destroy(module_name)
  else
    UnityModuleGenerator.new(options).generate(module_name)
  end

end


