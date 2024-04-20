#!/bin/bash
set -e


# Windows editor (release debug) 64 bit
scons production=yes tools=yes target=release_debug debug_symbols=no platform=windows bits=64 "$@" . 2>&1 | tee logs/windows_ed_64.log

# Windows editor (release debug) 32 bit
scons production=yes tools=yes target=release_debug debug_symbols=no platform=windows bits=32 "$@" . 2>&1 | tee logs/windows_ed_32.log

 # Windows templates 64 bit
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=windows bits=64 "$@" . 2>&1 | tee logs/windows_template_rd_64.log
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=windows bits=64 "$@" . 2>&1 | tee logs/windows_template_r_64.log

# Windows templates 32 bit
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=windows bits=32 "$@" . 2>&1 | tee logs/windows_template_rd_32.log
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=windows bits=32 "$@" . 2>&1 | tee logs/windows_template_r_32.log

