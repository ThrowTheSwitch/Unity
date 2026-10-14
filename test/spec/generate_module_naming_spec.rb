# =========================================================================
#   Unity - A Test Framework for C
#   ThrowTheSwitch.org
#   Copyright (c) 2007-26 Mike Karlesky, Mark VanderVoord, & Greg Williams
#   SPDX-License-Identifier: MIT
# =========================================================================

require '../auto/generate_module.rb'

# Each naming convention produces the case its help text names, whether the
# convention arrives as a string or, from YAML, as a symbol
describe 'UnityModuleGenerator naming' do
  def filename(naming, name = 'mad_science')
    UnityModuleGenerator.new(naming: naming).create_filename(name)
  end

  it 'makes bumpy names start with a capital' do
    expect(filename('bumpy')).to eq('MadScience')
  end

  it 'makes camel names start with a lowercase letter' do
    expect(filename('camel')).to eq('madScience')
  end

  it 'makes snake and caps names' do
    expect(filename('snake', 'MadScience')).to eq('mad_science')
    expect(filename('caps', 'MadScience')).to eq('MAD_SCIENCE')
  end

  it 'accepts a convention given as a symbol' do
    expect(filename(:bumpy)).to eq('MadScience')
    expect(filename(:snake, 'MadScience')).to eq('mad_science')
  end

  it 'keeps a name as given without a convention' do
    expect(filename(nil, 'MadScience')).to eq('MadScience')
  end
end
