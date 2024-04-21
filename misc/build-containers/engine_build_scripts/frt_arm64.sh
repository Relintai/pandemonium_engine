#!/bin/bash
set -e

# production=yes

# FRT templates arm64v8
scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt frt_arch=arm64v8 frt_cross=auto "$@"
scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt frt_arch=arm64v8 frt_cross=auto "$@"

# FRT SDL templates arm64v8
scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt_sdl frt_arch=arm64v8 frt_cross=auto "$@"
scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt_sdl frt_arch=arm64v8 frt_cross=auto "$@"

