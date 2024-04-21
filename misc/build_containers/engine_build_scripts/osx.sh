#!/bin/bash
set -e

# OSX editor
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=osx arch=x86_64 "$@" osxcross_sdk=darwin23.3
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=osx arch=arm64 "$@" osxcross_sdk=darwin23.3

# OSX templates release_debug
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=osx arch=x86_64 "$@" osxcross_sdk=darwin23.3
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=osx arch=arm64 "$@" osxcross_sdk=darwin23.3 

# OSX templates release
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=osx arch=x86_64 "$@" osxcross_sdk=darwin23.3
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=osx arch=arm64 "$@" osxcross_sdk=darwin23.3 

# OSX lipo

pushd misc/osx

./lipo.sh

popd


