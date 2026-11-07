#!/bin/bash
set -e

export PATH="${PANDEMONIUN_SDK_LINUX_ARMV6}/bin:${BASE_PATH}"

# FRT SDL templates arm32v6
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt_sdl frt_toolchain_arch_override=arm32v6 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt_sdl frt_toolchain_arch_override=arm32v6 "$@"

export PATH="${PANDEMONIUN_SDK_LINUX_ARMV7}/bin:${BASE_PATH}"

# FRT SDL templates arm32v7
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt_sdl frt_toolchain_arch_override=arm32v7 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt_sdl frt_toolchain_arch_override=arm32v7 "$@"

export PATH="${PANDEMONIUN_SDK_LINUX_ARM64}/bin:${BASE_PATH}"

# FRT SDL templates arm64v8
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt_sdl frt_toolchain_arch_override=arm64v8 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt_sdl frt_toolchain_arch_override=arm64v8 "$@"

# FRT SDL templates x86_32
export PATH="${PANDEMONIUN_SDK_LINUX_X86_32}/bin:${BASE_PATH}"

scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt_sdl frt_toolchain_arch_override=x86_32 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt_sdl frt_toolchain_arch_override=x86_32 "$@"

# FRT SDL templates x86_64
export PATH="${PANDEMONIUN_SDK_LINUX_X86_64}/bin:${BASE_PATH}"

scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt_sdl frt_toolchain_arch_override=x86_64 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt_sdl frt_toolchain_arch_override=x86_64 "$@"

