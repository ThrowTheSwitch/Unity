###################################################################################
#                                                                                 #
# NAME: meson.build                                                               #
#                                                                                 #
# AUTHOR: Mike Karlesky, Mark VanderVoord, Greg Williams.                         #
# WRITTEN BY: Michael Brockus.                                                    #
#                                                                                 #
# License: MIT                                                                    #
#                                                                                 #
###################################################################################



project('unity', 'c',
    version         : '2.4.3',
    license         : 'MIT',
    meson_version   : '>=0.50.0',
    default_options : 
    [ 
        'werror=true',
        'optimization=3', 
        'warning_level=3', 
        'b_sanitize=address,undefined', 
        'b_lto=true', 
        'b_lundef=true'
    ])
cc = meson.get_compiler('c')
args_for_langs = 'c'

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

subdir('src')

unity_dep = declare_dependency(
    version: meson.project_version(),
    link_with: unity_lib,
    include_directories: unity_dir)