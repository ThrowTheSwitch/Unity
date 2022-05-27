# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ==========================================

require 'yaml'

module YamlHelper
  def self.load(body)
    YAML.respond_to?(:unsafe_load) ?
      YAML.unsafe_load(body) : YAML.load(body)
  end

  def self.load_file(file)
    body = File.read(file)
    self.load(body)
  end
end
