# =========================================================================
#   Unity - A Test Framework for C
#   ThrowTheSwitch.org
#   Copyright (c) 2007-24 Mike Karlesky, Mark VanderVoord, & Greg Williams
#   SPDX-License-Identifier: MIT
# =========================================================================

module TypeSanitizer
  def self.sanitize_c_identifier(unsanitized)
    # convert filename to valid C identifier by replacing invalid chars with '_'
    unsanitized.gsub(/[-\/\\.,\s]/, '_')
  end
end
