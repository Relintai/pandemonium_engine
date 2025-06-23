#!/bin/bash
set -e

source /root/emsdk/emsdk_env.sh

# Javascript editor
scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no threads_enabled=yes platform=javascript "$@"

# Javascript templates normal
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=javascript "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=javascript "$@"

# TODO lto (production=yes) doesn't seem to work for now with thread builds

# Javascript templates threads
scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no threads_enabled=yes platform=javascript "$@"
scons tools=no target=release custom_modules_shared=no debug_symbols=no threads_enabled=yes platform=javascript "$@"

# Javascript templates gdnative
scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no gdnative_enabled=yes platform=javascript "$@"
scons tools=no target=release custom_modules_shared=no debug_symbols=no gdnative_enabled=yes platform=javascript "$@"

# Javascript templates threads + gdnative
scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no threads_enabled=yes gdnative_enabled=yes platform=javascript "$@"
scons tools=no target=release custom_modules_shared=no debug_symbols=no threads_enabled=yes gdnative_enabled=yes platform=javascript "$@"

