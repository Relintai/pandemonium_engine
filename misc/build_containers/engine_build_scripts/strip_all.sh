#!/bin/bash
set -e

cd bin

# This script only strips the release template binaries, but not the release_debug ones.
# This should improve their usefulness, and also differentiate them a bit more.
# The editor binaries are always stripped to make them smaller.

# If you want to strip all, you can:

# Comment out "set -e", so it does not error out on non zero exit code

#strip -v -s pandemonium.x11.*
#strip -v -s pandemonium_server.x11.*
#strip -v -s pandemonium.windows.*
#strip -v -s pandemonium_server.*
#strip -v -s pandemonium_http_server.*


export PATH="${PANDEMONIUN_SDK_LINUX_ARMV6}/bin:${BASE_PATH}"

# export templates
strip -v -s pandemonium.frt.opt.debug.arm32v6
strip -v -s pandemonium.frt_sdl.opt.debug.arm32v6

export PATH="${PANDEMONIUN_SDK_LINUX_ARMV7}/bin:${BASE_PATH}"

# export templates
strip -v -s pandemonium.frt.opt.debug.arm32v7
strip -v -s pandemonium.frt_sdl.opt.debug.arm32v7
strip -v -s pandemonium.x11.opt.debug.arm
strip -v -s pandemonium_http_server.x11.opt.debug.arm
strip -v -s pandemonium_server.x11.opt.debug.arm

# editor
strip -v -s pandemonium.x11.opt.tools.arm
strip -v -s pandemonium_server.x11.opt.tools.arm


export PATH="${PANDEMONIUN_SDK_LINUX_ARM64}/bin:${BASE_PATH}"

# export templates
strip -v -s pandemonium.frt.opt.debug.arm64v8
strip -v -s pandemonium.frt_sdl.opt.debug.arm64v8
strip -v -s pandemonium.x11.opt.debug.arm64
strip -v -s pandemonium_http_server.x11.opt.debug.arm64
strip -v -s pandemonium_server.x11.opt.debug.arm64

# editor
strip -v -s pandemonium.x11.opt.tools.arm64
strip -v -s pandemonium_server.x11.opt.tools.arm64

export PATH="${PANDEMONIUN_SDK_LINUX_X86_32}/bin:${BASE_PATH}"

# export templates
strip -v -s pandemonium.frt_sdl.opt.debug.x86_32 #ch
strip -v -s pandemonium.windows.opt.debug.32.exe
strip -v -s pandemonium.x11.opt.debug.32
strip -v -s pandemonium_http_server.x11.opt.debug.32
strip -v -s pandemonium_server.x11.opt.debug.32

# editor
strip -v -s pandemonium.windows.opt.tools.32.exe
strip -v -s pandemonium.x11.opt.tools.32
strip -v -s pandemonium_server.x11.opt.tools.32

export PATH="${PANDEMONIUN_SDK_LINUX_X86_64}/bin:${BASE_PATH}"

# export templates
strip -v -s pandemonium.frt_sdl.opt.debug.x86_64 #ch
strip -v -s pandemonium.windows.opt.debug.64.exe
strip -v -s pandemonium.x11.opt.debug.64
strip -v -s pandemonium_http_server.x11.opt.debug.64
strip -v -s pandemonium_server.x11.opt.debug.64

# editor
strip -v -s pandemonium.windows.opt.tools.64.exe
strip -v -s pandemonium.x11.opt.tools.64
strip -v -s pandemonium_server.x11.opt.tools.64

