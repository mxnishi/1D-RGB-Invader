#!/usr/bin/env python3
"""Add Arduino-style forward declarations; compile the sketch for this host."""
import pathlib,re,subprocess
root=pathlib.Path(__file__).resolve().parent
src=(root/'firmware.ino').read_text()
prototypes=[]
for line in src.splitlines():
    if re.match(r'^(?:void|bool|int|float|String|CRGB|Melody)\s+\w+\([^;]*\)\s*\{',line):
        prototypes.append(line.split('{',1)[0].strip()+';')
# Types are declared before the first function; forward declarations go there.
pos=src.index('CRGB hexToCRGB(')
(root/'sketch.generated.cpp').write_text('#include "Arduino.h"\n'+src[:pos]+'\n'.join(prototypes)+'\n'+src[pos:])
subprocess.run(['clang++','-std=c++17','-O1','-Wno-c++20-designator','-Wno-c++11-narrowing','-I'+str(root/'shims'),str(root/'host.cpp'),'-o',str(root/'emulator')],check=True)
print('Compiled fork sketch with host peripheral shims.')
