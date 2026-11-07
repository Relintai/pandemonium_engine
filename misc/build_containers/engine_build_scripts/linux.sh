#!/bin/bash
set -e

export PATH="${PANDEMONIUN_SDK_LINUX_X86_64}/bin:${BASE_PATH}"

# Linux editor 64 bit
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=64 "$@"

# Linux templates 64 bit
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=64 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=x11 bits=64 "$@"

export PATH="${PANDEMONIUN_SDK_LINUX_X86_32}/bin:${BASE_PATH}"

# Linux editor 32 bit
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=32 "$@"

# Linux templates 32 bit
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=32 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=x11 bits=32 "$@"

export PATH="${PANDEMONIUN_SDK_LINUX_ARM64}/bin:${BASE_PATH}"

# Linux editor arm64
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 arch=arm64 "$@"

# Linux templates arm64
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 arch=arm64 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=x11 arch=arm64 "$@"

# = arm32 template
export PATH="${PANDEMONIUN_SDK_LINUX_ARMV7}/bin:${BASE_PATH}"

# Linux editor arm
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 arch=arm "$@"

# Linux templates arm
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 arch=arm "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=x11 arch=arm "$@"


