#!/bin/bash
set -e

source /root/emsdk/emsdk_env.sh


# Javascript editor
scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no threads_enabled=yes platform=javascript "$@" . 2>&1 | tee logs/javascript_ed.log

# Javascript templates normal
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=javascript "$@" . 2>&1 | tee logs/javascript_rd.log

scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=javascript "$@" . 2>&1 | tee logs/javascript_r.log

# TODO lto (production=yes) doesn't seem to work for now with thread builds

# Javascript templates threads
scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no threads_enabled=yes platform=javascript "$@" . 2>&1 | tee logs/javascript_rd_threads.log

scons tools=no target=release custom_modules_shared=no debug_symbols=no threads_enabled=yes platform=javascript "$@" . 2>&1 | tee logs/javascript_r_threads.log

# Javascript templates gdnative
scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no gdnative_enabled=yes platform=javascript "$@" . 2>&1 | tee logs/javascript_rd_gdnative.log

scons tools=no target=release custom_modules_shared=no debug_symbols=no gdnative_enabled=yes platform=javascript "$@" . 2>&1 | tee logs/javascript_r_gdnative.log

