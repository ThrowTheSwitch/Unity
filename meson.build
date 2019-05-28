###################################################################################
#                                                                                 #
# NAME: meson.build                                                               #
#                                                                                 #
# AUTHOR: Squidfarts Team.                                                        #
#                                                                                 #
# CONTACT: <mailto:michael@squidfarts.com>                                        #
#                                                                                 #
# NOTICES:                                                                        #
#                                                                                 #
# MIT License                                                                     #
#                                                                                 #
# Copyright (c) 2019 Micheal Brockus                                              #
#                                                                                 #
# Permission is hereby granted, free of charge, to any person obtaining a copy    #
# of this software and associated documentation files (the "Software"), to deal   #
# in the Software without restriction, including without limitation the rights    #
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       #
# copies of the Software, and to permit persons to whom the Software is           #
# furnished to do so, subject to the following conditions:                        #
#                                                                                 #
# The above copyright notice and this permission notice shall be included in all  #
# copies or substantial portions of the Software.                                 #
#                                                                                 #
# DISCLAIMERS:                                                                    #
#                                                                                 #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      #
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     #
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          #
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   #
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   #
# SOFTWARE.                                                                       #
#                                                                                 #
###################################################################################



##
#
# Meson: Declaration of project.
#
##
project('unity', 'c',
    version         : '2.4.3',
    license         : 'MIT',
    meson_version   : '>=0.50.0',
    default_options : 
        [ 
            'warning_level=3', 
            'werror=true', 
            'b_sanitize=undefined', 
            'b_lto=true', 
            'b_lundef=true', 
            'c_std=c11' 
        ]
)
cc = meson.get_compiler('c')
args_for_langs = 'c'



##
#
# Meson: Add compiler flags
#
##
if cc.get_id() == 'clang'
    add_project_arguments(
        '-Wweak-vtables', 
        '-Wexit-time-destructors',
        '-Wglobal-constructors', 
        '-Wmissing-noreturn', language: args_for_langs)
endif

if cc.get_argument_syntax() == 'gcc'

    add_project_arguments(
        '-Wall', 
        '-Wextra', 
        '-Wunreachable-code', 
        '-Wmissing-declarations',
        '-Wmissing-prototypes',
        '-Wredundant-decls',
        '-Wundef',
        '-Wwrite-strings',
        '-Wformat',
        '-Wformat-nonliteral',
        '-Wformat-security',
        '-Wold-style-definition',
        '-Winit-self',
        '-Wmissing-include-dirs',
        '-Waddress',
        '-Waggregate-return',
        '-Wno-multichar',
        '-Wdeclaration-after-statement',
        '-Wvla',
        '-Wpointer-arith',language: args_for_langs)
endif

if cc.get_id() == 'msvc'
    add_project_arguments(
        '/W4', 
        '/w44265', 
        '/w44061', 
        '/w44062', 
        '/wd4018', # implicit signed/unsigned conversion
        '/wd4146', # unary minus on unsigned (beware INT_MIN)
        '/wd4244', # lossy type conversion (e.g. double -> int)
        '/wd4305', # truncating type conversion (e.g. double -> float)
        mesno.get_supported_arguments(['/utf-8']), language: args_for_langs)
endif



##
#
# Meson: Add subdirectories
#
##
subdir('src')

unity_dep = declare_dependency(
    version: meson.project_version(),
    link_with: unity_lib,
    include_directories: unity_dir)