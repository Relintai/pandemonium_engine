#!/bin/bash
set -e


# Windows editor (release debug) 64 bit
scons production=yes tools=yes target=release_debug debug_symbols=no platform=windows bits=64 "$@"

# Windows editor (release debug) 32 bit
scons production=yes tools=yes target=release_debug debug_symbols=no platform=windows bits=32 "$@"

 # Windows templates 64 bit
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=windows bits=64 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=windows bits=64 "$@"

# Windows templates 32 bit
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=windows bits=32 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=windows bits=32 "$@"

