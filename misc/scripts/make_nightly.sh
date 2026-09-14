#!/bin/bash
set -e

# Run this script form it's own folder

# Before using this script, I recommend using:
# pre_release_check_files_present.sh
# strip_all.sh

cd ../..
project_root=$(pwd)

full_project_name=$1

rm -Rf ./release
mkdir release

mkdir release/temp
mkdir release/temp/templates
mkdir release/temp/templates_server
mkdir release/temp/templates_sbc
mkdir release/temp/gdnative_headers

# ==== Copy Normal Templates ====

# Windows

cp bin/pandemonium.windows.opt.32.exe release/temp/templates/windows_32_release.exe
cp bin/pandemonium.windows.opt.64.exe release/temp/templates/windows_64_release.exe

cp bin/pandemonium.windows.opt.debug.32.exe release/temp/templates/windows_32_debug.exe
cp bin/pandemonium.windows.opt.debug.64.exe release/temp/templates/windows_64_debug.exe

# Linux

cp bin/pandemonium.x11.opt.32 release/temp/templates/linux_x11_32_release
cp bin/pandemonium.x11.opt.64 release/temp/templates/linux_x11_64_release
cp bin/pandemonium.x11.opt.arm release/temp/templates/linux_x11_arm32_release
cp bin/pandemonium.x11.opt.arm64 release/temp/templates/linux_x11_arm64_release

cp bin/pandemonium.x11.opt.debug.32 release/temp/templates/linux_x11_32_debug
cp bin/pandemonium.x11.opt.debug.64 release/temp/templates/linux_x11_64_debug
cp bin/pandemonium.x11.opt.debug.arm release/temp/templates/linux_x11_arm32_debug
cp bin/pandemonium.x11.opt.debug.arm64 release/temp/templates/linux_x11_arm64_debug

# Server (Linux)

cp bin/pandemonium_server.x11.opt.32 release/temp/templates_server/linux_server_32_release
cp bin/pandemonium_server.x11.opt.64 release/temp/templates_server/linux_server_64_release
cp bin/pandemonium_server.x11.opt.arm release/temp/templates_server/linux_server_arm32_release
cp bin/pandemonium_server.x11.opt.arm64 release/temp/templates_server/linux_server_arm64_release

cp bin/pandemonium_server.x11.opt.debug.32 release/temp/templates_server/linux_server_32_debug
cp bin/pandemonium_server.x11.opt.debug.64 release/temp/templates_server/linux_server_64_debug
cp bin/pandemonium_server.x11.opt.debug.arm release/temp/templates_server/linux_server_arm32_debug
cp bin/pandemonium_server.x11.opt.debug.arm64 release/temp/templates_server/linux_server_arm64_debug

# HTTP Server (Linux)

cp bin/pandemonium_http_server.x11.opt.32 release/temp/templates_server/linux_http_server_32_release
cp bin/pandemonium_http_server.x11.opt.64 release/temp/templates_server/linux_http_server_64_release
cp bin/pandemonium_http_server.x11.opt.arm release/temp/templates_server/linux_http_server_arm32_release
cp bin/pandemonium_http_server.x11.opt.arm64 release/temp/templates_server/linux_http_server_arm64_release

cp bin/pandemonium_http_server.x11.opt.debug.32 release/temp/templates_server/linux_http_server_32_debug
cp bin/pandemonium_http_server.x11.opt.debug.64 release/temp/templates_server/linux_http_server_64_debug
cp bin/pandemonium_http_server.x11.opt.debug.arm release/temp/templates_server/linux_http_server_arm32_debug
cp bin/pandemonium_http_server.x11.opt.debug.arm64 release/temp/templates_server/linux_http_server_arm64_debug

# JS

cp bin/pandemonium.javascript.opt.zip release/temp/templates/webassembly_release.zip
cp bin/pandemonium.javascript.opt.threads.zip release/temp/templates/webassembly_threads_release.zip
cp bin/pandemonium.javascript.opt.gdnative.zip release/temp/templates/webassembly_gdnative_release.zip
cp bin/pandemonium.javascript.opt.gdnative.threads.zip release/temp/templates/webassembly_gdnative_threads_release.zip

cp bin/pandemonium.javascript.opt.debug.zip release/temp/templates/webassembly_debug.zip
cp bin/pandemonium.javascript.opt.debug.threads.zip release/temp/templates/webassembly_threads_debug.zip
cp bin/pandemonium.javascript.opt.debug.gdnative.zip release/temp/templates/webassembly_gdnative_debug.zip
cp bin/pandemonium.javascript.opt.debug.gdnative.threads.zip release/temp/templates/webassembly_gdnative_threads_debug.zip

# Android
cp bin/android_source.zip release/temp/templates/android_source.zip

cp bin/android_release.apk release/temp/templates/android_release.apk
cp bin/android_debug.apk release/temp/templates/android_debug.apk

cp bin/pandemonium-lib.release.aar release/temp/templates/android_pandemonium-lib.release.aar
cp bin/pandemonium-lib.debug.aar release/temp/templates/android_pandemonium-lib.debug.aar

# OSX - export templates

cp bin/osx.zip release/temp/templates/osx.zip

# IOS
cp bin/iphone.zip release/temp/templates/iphone.zip

# FRT

cp bin/pandemonium.frt.opt.arm32v6 release/temp/templates_sbc/linux_frt_arm32v6_release
cp bin/pandemonium.frt.opt.arm32v7 release/temp/templates_sbc/linux_frt_arm32v7_release
cp bin/pandemonium.frt.opt.arm64v8 release/temp/templates_sbc/linux_frt_arm64v8_release

cp bin/pandemonium.frt.opt.debug.arm32v6 release/temp/templates_sbc/linux_frt_arm32v6_debug
cp bin/pandemonium.frt.opt.debug.arm32v7 release/temp/templates_sbc/linux_frt_arm32v7_debug
cp bin/pandemonium.frt.opt.debug.arm64v8 release/temp/templates_sbc/linux_frt_arm64v8_debug

# FRT SDL

cp bin/pandemonium.frt_sdl.opt.arm32v6 release/temp/templates_sbc/linux_frt_sdl_arm32v6_release
cp bin/pandemonium.frt_sdl.opt.arm32v7 release/temp/templates_sbc/linux_frt_sdl_arm32v7_release
cp bin/pandemonium.frt_sdl.opt.arm64v8 release/temp/templates_sbc/linux_frt_sdl_arm64v8_release

cp bin/pandemonium.frt_sdl.opt.debug.arm32v6 release/temp/templates_sbc/linux_frt_sdl_arm32v6_debug
cp bin/pandemonium.frt_sdl.opt.debug.arm32v7 release/temp/templates_sbc/linux_frt_sdl_arm32v7_debug
cp bin/pandemonium.frt_sdl.opt.debug.arm64v8 release/temp/templates_sbc/linux_frt_sdl_arm64v8_debug

# ==== Copy Enditors ====

# Windows

cp bin/pandemonium.windows.opt.tools.32.exe release/temp/pandemonium_nightly_win32.exe
cp bin/pandemonium.windows.opt.tools.64.exe release/temp/pandemonium_nightly_win64.exe

# Linux

cp bin/pandemonium.x11.opt.tools.32 release/temp/pandemonium_nightly_linux_32
cp bin/pandemonium.x11.opt.tools.64 release/temp/pandemonium_nightly_linux_64
cp bin/pandemonium.x11.opt.tools.arm release/temp/pandemonium_nightly_linux_arm32
cp bin/pandemonium.x11.opt.tools.arm64 release/temp/pandemonium_nightly_linux_arm64

# Server (Linux)

cp bin/pandemonium_server.x11.opt.tools.32 release/temp/pandemonium_nightly_linux_server_32
cp bin/pandemonium_server.x11.opt.tools.64 release/temp/pandemonium_nightly_linux_server_64
cp bin/pandemonium_server.x11.opt.tools.arm release/temp/pandemonium_nightly_linux_server_arm32
cp bin/pandemonium_server.x11.opt.tools.arm64 release/temp/pandemonium_nightly_linux_server_arm64

# JS

cp bin/pandemonium.javascript.opt.tools.threads.zip release/temp/pandemonium_nightly_javascript.zip
#cp bin/pandemonium.javascript.opt.tools.gdnative.threads.zip release/temp/pandemonium_nightly_javascript_gdnative.zip

# Android

cp bin/android_editor.apk release/temp/pandemonium_nightly_android.apk

# OSX - Editor

cp -r bin/Pandemonium.app release/temp/pandemonium_nightly_osx.app

# ==== Copy sources ====

mkdir release/temp/pandemonium_source

python ./misc/scripts_app/copy_repos.py . ./release/temp/pandemonium_source 

# ==== Create version.txt for the export templates ====

#./bin/pandemonium.x11.opt.tools.64 --version-full-config > release/temp/templates/version.txt
#./bin/pandemonium.x11.opt.tools.64 --version-full-config > release/temp/templates_server/version.txt
#./bin/pandemonium.x11.opt.tools.64 --version-full-config > release/temp/templates_sbc/version.txt

./bin/pandemonium_server.x11.opt.tools.64 --version-full-config > release/temp/templates/version.txt
./bin/pandemonium_server.x11.opt.tools.64 --version-full-config > release/temp/templates_server/version.txt
./bin/pandemonium_server.x11.opt.tools.64 --version-full-config > release/temp/templates_sbc/version.txt
./bin/pandemonium_server.x11.opt.tools.64 --version > release/version.txt

# ==== GDNative headers

# Copy GDNative includes

cp -r ./release/temp/pandemonium_source/modules/gdnative/include/*  release/temp/gdnative_headers/

# Copy gdnative_api.json

cp ./release/temp/pandemonium_source/modules/gdnative/gdnative_api.json release/temp/gdnative_headers/gdnative_api.json

# Copy gdnative_api_struct.gen.h

cp bin/gdnative_api_struct.gen.h release/temp/gdnative_headers/gdnative_api_struct.gen.h

# Generate JSON dump of the Pandemonium API for GDNative bindings.

#./bin/pandemonium.x11.opt.tools.64 --gdnative-generate-json-api release/temp/gdnative_headers/api.json

./bin/pandemonium_server.x11.opt.tools.64 --gdnative-generate-json-api release/temp/gdnative_headers/api.json

# ==== Zip everything ====

cd release/temp

# Export templates

zip -q -r ../pandemonium_nightly_export_templates.zip  ./templates/*
zip -q -r ../pandemonium_nightly_export_templates_server.zip  ./templates_server/*
zip -q -r ../pandemonium_nightly_export_templates_sbc.zip  ./templates_sbc/*

# GDNative headers

zip -q -r ../pandemonium_nightly_gdnative_headers.zip  ./gdnative_headers/*

# == Editors ==

# Windows

zip -q ../pandemonium_nightly_win32.zip ./pandemonium_nightly_win32.exe
zip -q ../pandemonium_nightly_win64.zip ./pandemonium_nightly_win64.exe

# Linux

zip -q ../pandemonium_nightly_linux_32.zip ./pandemonium_nightly_linux_32
zip -q ../pandemonium_nightly_linux_64.zip ./pandemonium_nightly_linux_64
zip -q ../pandemonium_nightly_linux_arm32.zip ./pandemonium_nightly_linux_arm32
zip -q ../pandemonium_nightly_linux_arm64.zip ./pandemonium_nightly_linux_arm64

# Server (Linux)

zip -q ../pandemonium_nightly_linux_server_32.zip ./pandemonium_nightly_linux_server_32
zip -q ../pandemonium_nightly_linux_server_64.zip ./pandemonium_nightly_linux_server_64
zip -q ../pandemonium_nightly_linux_server_arm32.zip ./pandemonium_nightly_linux_server_arm32
zip -q ../pandemonium_nightly_linux_server_arm64.zip ./pandemonium_nightly_linux_server_arm64


# Android

zip -q ../pandemonium_nightly_android.zip ./pandemonium_nightly_android.apk

# OSX - Editor

zip -r -q ../pandemonium_nightly_osx.app.zip ./pandemonium_nightly_osx.app

# Sources

zip -r -q ../pandemonium_source.zip ./pandemonium_source

cd ..

# JS

cp ./temp/pandemonium_nightly_javascript.zip pandemonium_nightly_javascript.zip 

# ==== Rename export temlpates to tpz ====
# It's still a normal zip though
# Maybe it should be just kept as a normal zip?

mv pandemonium_nightly_export_templates.zip pandemonium_nightly_export_templates.tpz
mv pandemonium_nightly_export_templates_server.zip pandemonium_nightly_export_templates_server.tpz
mv pandemonium_nightly_export_templates_sbc.zip pandemonium_nightly_export_templates_sbc.tpz


# Will complatin about the temp directory, but that's fine.
md5sum * > md5_sums.txt

