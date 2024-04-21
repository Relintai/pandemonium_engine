#!/bin/bash
set -e

# Android editor
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=armv7 "$@"
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=arm64v8 "$@" 
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=x86 "$@"
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=x86_64 "$@"

pushd platform/android/java/

./gradlew generatePandemoniumEditor 

popd

# Android templates release_debug
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=armv7 "$@"
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=arm64v8 "$@"
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=x86 "$@"
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=x86_64 "$@"

pushd platform/android/java/

./gradlew generatePandemoniumTemplates

popd

# Android templates release
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=android android_arch=armv7 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=android android_arch=arm64v8 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=android android_arch=x86 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=android android_arch=x86_64 "$@"

pushd platform/android/java/

./gradlew generatePandemoniumTemplates 

popd

