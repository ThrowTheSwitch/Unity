#!/usr/bin/env python3
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

