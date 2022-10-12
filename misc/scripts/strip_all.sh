#!/bin/bash
set -e

cd ../../bin/

# Todo figure out a way to also strip the pi4 versions (from x86 linux)

strip pandemonium.x11.*
strip pandemonium_server.x11.*
strip pandemonium.windows.*
