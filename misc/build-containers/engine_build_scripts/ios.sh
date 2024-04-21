#!/bin/bash
set -e

export IOS_SDK="17.2"
export IOS_LIPO="/root/ioscross/arm64/bin/arm-apple-darwin11-lipo"
#export OPTIONS="platform=iphone production=yes use_lto=none"
export OPTIONS="platform=iphone"

# arm64 device
scons $OPTIONS arch=arm64 ios_simulator=no tools=no target=release_debug \
	IPHONESDK="/root/ioscross/arm64/SDK/iPhoneOS${IOS_SDK}.sdk" IPHONEPATH="/root/ioscross/arm64/" ios_triple="arm-apple-darwin11-" "$@"

scons $OPTIONS arch=arm64 ios_simulator=no tools=no target=release \
	IPHONESDK="/root/ioscross/arm64/SDK/iPhoneOS${IOS_SDK}.sdk" IPHONEPATH="/root/ioscross/arm64/" ios_triple="arm-apple-darwin11-" "$@"

# arm64 simulator
# Disabled for now as it doesn't work with cctools-port and current LLVM.
# See https://github.com/godotengine/build-containers/pull/85.
# scons platform=ios $OPTIONS arch=arm64 ios_simulator=yes target=template_debug \
#  IOS_SDK_PATH="/root/ioscross/arm64_sim/SDK/iPhoneSimulator${IOS_SDK}.sdk" IOS_TOOLCHAIN_PATH="/root/ioscross/arm64_sim/" ios_triple="arm-apple-darwin11-"
# scons platform=ios $OPTIONS arch=arm64 ios_simulator=yes target=template_release \
#  IOS_SDK_PATH="/root/ioscross/arm64_sim/SDK/iPhoneSimulator${IOS_SDK}.sdk" IOS_TOOLCHAIN_PATH="/root/ioscross/arm64_sim/" ios_triple="arm-apple-darwin11-"

# x86_64 simulator
scons $OPTIONS arch=x86_64 ios_simulator=yes tools=no target=release_debug \
	IPHONESDK="/root/ioscross/x86_64_sim/SDK/iPhoneSimulator${IOS_SDK}.sdk" IPHONEPATH="/root/ioscross/x86_64_sim/" ios_triple="x86_64-apple-darwin11-" "$@"

scons $OPTIONS arch=x86_64 ios_simulator=yes tools=no target=release \
	IPHONESDK="/root/ioscross/x86_64_sim/SDK/iPhoneSimulator${IOS_SDK}.sdk" IPHONEPATH="/root/ioscross/x86_64_sim/" ios_triple="x86_64-apple-darwin11-" "$@"

#cp bin/libgodot.ios.template_release.arm64.a /root/out/templates/libgodot.ios.a
#cp bin/libgodot.ios.template_debug.arm64.a /root/out/templates/libgodot.ios.debug.a
#$IOS_LIPO -create bin/libgodot.ios.template_release.arm64.simulator.a bin/libgodot.ios.template_release.x86_64.simulator.a -output /root/out/templates/libgodot.ios.simulator.a
#$IOS_LIPO -create bin/libgodot.ios.template_debug.arm64.simulator.a bin/libgodot.ios.template_debug.x86_64.simulator.a -output /root/out/templates/libgodot.ios.debug.simulator.a
#cp bin/libgodot.ios.template_release.x86_64.simulator.a /root/out/templates/libgodot.ios.simulator.a
#cp bin/libgodot.ios.template_debug.x86_64.simulator.a /root/out/templates/libgodot.ios.debug.simulator.a

