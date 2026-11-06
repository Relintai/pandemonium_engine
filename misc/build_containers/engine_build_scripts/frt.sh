#!/bin/bash
set -e

export PATH="${PANDEMONIUN_SDK_LINUX_ARMV6}/bin:${BASE_PATH}"

# FRT templates arm32v6
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt "$@"

# FRT SDL templates arm32v6
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt_sdl "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt_sdl "$@"

export PATH="${PANDEMONIUN_SDK_LINUX_ARMV7}/bin:${BASE_PATH}"

# FRT templates arm32v7
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt "$@"

# FRT SDL templates arm32v7
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt_sdl "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt_sdl "$@"

export PATH="${PANDEMONIUN_SDK_LINUX_ARM64}/bin:${BASE_PATH}"

# FRT templates arm64v8
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt "$@"

# FRT SDL templates arm64v8
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt_sdl "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt_sdl "$@"


export PATH="${PANDEMONIUN_SDK_LINUX_X86_64}/bin:${BASE_PATH}"

scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt_sdl "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt_sdl "$@"

export PATH="${PANDEMONIUN_SDK_LINUX_X86_32}/bin:${BASE_PATH}"

scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt_sdl "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt_sdl "$@"

