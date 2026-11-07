#!/bin/bash
set -e

export PATH="${PANDEMONIUN_SDK_LINUX_ARMV6}/bin:${BASE_PATH}"

# FRT templates arm32v6
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt frt_toolchain_arch_override=arm32v6 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt frt_toolchain_arch_override=arm32v6 "$@"

export PATH="${PANDEMONIUN_SDK_LINUX_ARMV7}/bin:${BASE_PATH}"

# FRT templates arm32v7
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt frt_toolchain_arch_override=arm32v7 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt frt_toolchain_arch_override=arm32v7 "$@"

export PATH="${PANDEMONIUN_SDK_LINUX_ARM64}/bin:${BASE_PATH}"

# FRT templates arm64v8
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=frt frt_toolchain_arch_override=arm64v8 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=frt frt_toolchain_arch_override=arm64v8 "$@"

