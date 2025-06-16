#!/bin/bash
set -e

export IOS_SDK="18.5"
export IOS_LIPO="/root/ioscross/arm64/bin/arm-apple-darwin11-lipo"
#export OPTIONS="platform=iphone production=yes use_lto=none"
export IOS_DEVICE="IOS_SDK_PATH=/root/SDKs/iPhoneOS${IOS_SDK}.sdk"
export IOS_SIMULATOR="IOS_SDK_PATH=/root/SDKs/iPhoneSimulator${IOS_SDK}.sdk simulator=yes"
export APPLE_TARGET_ARM64="APPLE_TOOLCHAIN_PATH=/root/ioscross/arm64 apple_target_triple=arm-apple-darwin11-"
export APPLE_TARGET_X86_64="APPLE_TOOLCHAIN_PATH=/root/ioscross/x86_64 apple_target_triple=x86_64-apple-darwin11-"

export OPTIONS="platform=iphone"

# arm64 device
scons $OPTIONS arch=arm64 ios_simulator=no tools=no target=release_debug $IOS_DEVICE $APPLE_TARGET_ARM64 "$@"
scons $OPTIONS arch=arm64 ios_simulator=no tools=no target=release $IOS_DEVICE $APPLE_TARGET_ARM64 "$@"

# arm64 simulator
# Disabled for now as it doesn't work with cctools-port and current LLVM.
# See https://github.com/godotengine/build-containers/pull/85.
# scons platform=ios $OPTIONS arch=arm64 ios_simulator=yes target=template_debug $IOS_SIMULATOR $APPLE_TARGET_ARM64
# scons platform=ios $OPTIONS arch=arm64 ios_simulator=yes target=template_release $IOS_SIMULATOR $APPLE_TARGET_ARM64

# x86_64 simulator
scons $OPTIONS arch=x86_64 ios_simulator=yes tools=no target=release_debug $IOS_SIMULATOR $APPLE_TARGET_X86_64 "$@"

scons $OPTIONS arch=x86_64 ios_simulator=yes tools=no target=release $IOS_SIMULATOR $APPLE_TARGET_X86_64 "$@"

#cp bin/libgodot.ios.template_release.arm64.a /root/out/templates/libgodot.ios.a
#cp bin/libgodot.ios.template_debug.arm64.a /root/out/templates/libgodot.ios.debug.a
#$IOS_LIPO -create bin/libgodot.ios.template_release.arm64.simulator.a bin/libgodot.ios.template_release.x86_64.simulator.a -output /root/out/templates/libgodot.ios.simulator.a
#$IOS_LIPO -create bin/libgodot.ios.template_debug.arm64.simulator.a bin/libgodot.ios.template_debug.x86_64.simulator.a -output /root/out/templates/libgodot.ios.debug.simulator.a
#cp bin/libgodot.ios.template_release.x86_64.simulator.a /root/out/templates/libgodot.ios.simulator.a
#cp bin/libgodot.ios.template_debug.x86_64.simulator.a /root/out/templates/libgodot.ios.debug.simulator.a

