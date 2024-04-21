#!/bin/bash
set -e

# Android editor
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=armv7 "$@" . 2>&1 | tee logs/android_editor_armv7.log
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=arm64v8 "$@" . 2>&1 | tee logs/android_editor_arm64v8.log
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=x86 "$@" . 2>&1 | tee logs/android_editor_x86.log
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=x86_64 "$@" . 2>&1 | tee logs/android_editor_x86_64.log

pushd platform/android/java/

./gradlew generatePandemoniumEditor 

popd

# Android templates release_debug
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=armv7 "$@" . 2>&1 | tee logs/android_template_rd_armv7.log
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=arm64v8 "$@" . 2>&1 | tee logs/android_template_rd_arm64v8.log
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=x86 "$@" . 2>&1 | tee logs/android_template_rd_x86.log
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=x86_64 "$@" . 2>&1 | tee logs/android_template_rd_x86_64.log

pushd platform/android/java/

./gradlew generatePandemoniumTemplates

popd

# Android templates release
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=android android_arch=armv7 "$@" . 2>&1 | tee logs/android_template_r_armv7.log
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=android android_arch=arm64v8 "$@" . 2>&1 | tee logs/android_template_r_arm64v8.log
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=android android_arch=x86 "$@" . 2>&1 | tee logs/android_template_r_x86.log
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=android android_arch=x86_64 "$@" . 2>&1 | tee logs/android_template_r_x86_64.log

pushd platform/android/java/

./gradlew generatePandemoniumTemplates 

popd

