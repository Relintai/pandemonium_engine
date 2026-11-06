#!/bin/bash
set -e

export PATH="${PANDEMONIUN_SDK_LINUX_ARM64}/bin:${BASE_PATH}"

# FRT templates arm64v8
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt "$@"

# FRT SDL templates arm64v8
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt_sdl "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt_sdl "$@"

