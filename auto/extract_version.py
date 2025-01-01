#!/usr/bin/env python3
# =========================================================================
#   Unity - A Test Framework for C
#   ThrowTheSwitch.org
#   Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
#   SPDX-License-Identifier: MIT
# =========================================================================

import re
import sys

ver_re = re.compile(r"^#define\s+UNITY_VERSION_(?:MAJOR|MINOR|BUILD)\s+(\d+)$")
version = []

with open(sys.argv[1], "r") as f:
    for line in f:
        m = ver_re.match(line)
        if m:
            version.append(m.group(1))

print(".".join(version))

