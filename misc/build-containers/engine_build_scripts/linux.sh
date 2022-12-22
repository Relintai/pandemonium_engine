#!/bin/bash
set -e

# pkg-config wrongly points to lib instead of lib64 for arch-dependent header.
sed -i ${GODOT_SDK_LINUX_X86_64}/x86_64-godot-linux-gnu/sysroot/usr/lib/pkgconfig/dbus-1.pc -e "s@/lib@/lib64@g"

export PATH="${GODOT_SDK_LINUX_X86_64}/bin:${BASE_PATH}"

# Linux editor 64 bit
scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=64 "$@" . 2>&1 | tee logs/linux_ed_64.log

# Linux templates 64 bit
scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=64 "$@" . 2>&1 | tee logs/linux_template_rd_64.log
scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=x11 bits=64 "$@" . 2>&1 | tee logs/linux_template_r_64.log

# Linux headless (editor) 64bit
scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=server bits=64 "$@" . 2>&1 | tee logs/headless.log

# Linux server (templates) 64bit
scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=server bits=64 "$@" . 2>&1 | tee logs/server_rd.log
scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=server bits=64 "$@" . 2>&1 | tee logs/server_d.log

export PATH="${GODOT_SDK_LINUX_X86}/bin:${BASE_PATH}"

# Linux editor 32 bit
scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=32 "$@" . 2>&1 | tee logs/linux_ed_32.log

# Linux templates 32 bit
scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=32 "$@" . 2>&1 | tee logs/linux_template_rd_32.log
scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=x11 bits=32 "$@" . 2>&1 | tee logs/linux_template_r_32.log

export PATH="${GODOT_SDK_LINUX_ARMHF}/bin:${BASE_PATH}"

# Linux editor armhf (PI4)
scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 arch=armv7 CCFLAGS="-mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -mlittle-endian -munaligned-access" "$@" . 2>&1 | tee logs/linux_ed_armhf.log

# Linux templates armhf
scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 arch=armv7 CCFLAGS="-mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -mlittle-endian -munaligned-access" "$@" . 2>&1 | tee logs/linux_template_rd_armhf.log
scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=x11 arch=armv7 CCFLAGS="-mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -mlittle-endian -munaligned-access" "$@" . 2>&1 | tee logs/linux_template_r_armhf.log

# Rename armhf to pi4
# Todo think about a better naming convention, especially if FRT is added to the builds as well

mv -f bin/pandemonium.x11.opt.armv7 bin/pandemonium.x11.opt.pi4
mv -f bin/pandemonium.x11.opt.debug.armv7 bin/pandemonium.x11.opt.debug.pi4
mv -f bin/pandemonium.x11.opt.tools.armv7 bin/pandemonium.x11.opt.tools.pi4
