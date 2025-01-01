# =========================================================================
#   Unity - A Test Framework for C
#   ThrowTheSwitch.org
#   Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
#   SPDX-License-Identifier: MIT
# =========================================================================

require 'yaml'

module YamlHelper
  def self.load(body)
    if YAML.respond_to?(:unsafe_load)
      YAML.unsafe_load(body)
    else
      YAML.load(body)
    end
  end

  def self.load_file(file)
    body = File.read(file)
    self.load(body)
  end
end
