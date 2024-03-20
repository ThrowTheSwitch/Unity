# =========================================================================
#   Unity - A Test Framework for C
#   ThrowTheSwitch.org
#   Copyright (c) 2007-24 Mike Karlesky, Mark VanderVoord, & Greg Williams
#   SPDX-License-Identifier: MIT
# =========================================================================

import os

Import("env")

env.Append(CPPDEFINES=["UNITY_INCLUDE_CONFIG_H"])

# import "unity_config.h" folder to the library builder
try:
    Import("projenv")

    projenv.Append(CPPDEFINES=["UNITY_INCLUDE_CONFIG_H"])
    for p in projenv["CPPPATH"]:
        p = projenv.subst(p)
        if os.path.isfile(os.path.join(p, "unity_config.h")):
            env.Prepend(CPPPATH=[p])
except:
    pass
