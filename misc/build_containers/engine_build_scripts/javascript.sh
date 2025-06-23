#!/bin/bash
set -e

source /root/emsdk/emsdk_env.sh

# Javascript editor
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no threads_enabled=yes platform=javascript "$@"

# Javascript templates normal
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=javascript "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=javascript "$@"

# Javascript templates threads
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no threads_enabled=yes platform=javascript "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no threads_enabled=yes platform=javascript "$@"

# Javascript templates gdnative
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no gdnative_enabled=yes platform=javascript "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no gdnative_enabled=yes platform=javascript "$@"

# Javascript templates threads + gdnative
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no threads_enabled=yes gdnative_enabled=yes platform=javascript "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no threads_enabled=yes gdnative_enabled=yes platform=javascript "$@"

