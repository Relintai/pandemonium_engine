#!/bin/bash
set -e

# Run this script form it's own folder

# Before using this script, I recommend using:
# pre_release_check_files_present.sh
# strip_all.sh

cd ../..
project_root=$(pwd)

full_project_name=$1

version=""
version_snake_cased=""

if [ ! -z $1 ]; then
    version="."
    version+=$1

    version_snake_cased=${version//./_}
fi

rm -Rf ./release
mkdir release

mkdir release/temp
mkdir release/temp/templates


# ==== Copy Templates ====

# Windows (64 bit)
cp bin/pandemonium.windows.opt.64.exe release/temp/templates/windows_64_release.exe
cp bin/pandemonium.windows.opt.debug.64.exe release/temp/templates/windows_64_debug.exe

# Windows (32 bit)
cp bin/pandemonium.windows.opt.32.exe release/temp/templates/windows_32_release.exe
cp bin/pandemonium.windows.opt.debug.32.exe release/temp/templates/windows_32_debug.exe
  
# Linux (64 bit)
cp bin/pandemonium.x11.opt.64 release/temp/templates/linux_x11_64_release
cp bin/pandemonium.x11.opt.debug.64 release/temp/templates/linux_x11_64_debug

# Linux (32 bit)
cp bin/pandemonium.x11.opt.64 release/temp/templates/linux_x11_32_release
cp bin/pandemonium.x11.opt.debug.64 release/temp/templates/linux_x11_32_debug

# Pi4
cp bin/pandemonium.x11.opt.pi4 release/temp/templates/linux_x11_pi4_release
cp bin/pandemonium.x11.opt.debug.pi4 release/temp/templates/linux_x11_pi4_debug

# Server - Linux (64 bit)
cp bin/pandemonium_server.x11.opt.64 release/temp/templates/linux_server_x11_64_release
cp bin/pandemonium_server.x11.opt.debug.64 release/temp/templates/linux_server_x11_64_debug

# JS
cp bin/pandemonium.javascript.opt.zip release/temp/templates/webassembly_release.zip
cp bin/pandemonium.javascript.opt.debug.zip release/temp/templates/webassembly_debug.zip

cp bin/pandemonium.javascript.opt.threads.zip release/temp/templates/webassembly_threads_release.zip
cp bin/pandemonium.javascript.opt.debug.threads.zip release/temp/templates/webassembly_threads_debug.zip

# Android
cp bin/android_source.zip release/temp/templates/android_source.zip
cp bin/android_debug.apk release/temp/templates/android_debug.apk
cp bin/android_release.apk release/temp/templates/android_release.apk
cp bin/pandemonium-lib.release.aar release/temp/templates/android_pandemonium-lib.release.aar

# OSX - export templates
cp bin/osx.zip release/temp/templates/osx.zip

# ==== Copy Enditors ====

# Windows (64 bit)
cp bin/pandemonium.windows.opt.tools.64.exe release/temp/pandemonium_v${version_snake_cased}_stable_win64.exe

# Windows (32 bit)
cp bin/pandemonium.windows.opt.tools.32.exe release/temp/pandemonium_v${version_snake_cased}_stable_win32.exe
  
# Linux (64 bit)
cp bin/pandemonium.x11.opt.tools.64 release/temp/pandemonium_v${version_snake_cased}_stable_linux64

# Linux (32 bit)
cp bin/pandemonium.x11.opt.tools.64 release/temp/pandemonium_v${version_snake_cased}_stable_linux32

# Pi4
cp bin/pandemonium.x11.opt.tools.pi4 release/temp/pandemonium_v${version_snake_cased}_stable_pi4

# Server - Linux (64 bit)
cp bin/pandemonium_server.x11.opt.tools.64 release/temp/pandemonium_v${version_snake_cased}_stable_linux_server64

# JS
cp bin/pandemonium.javascript.opt.tools.threads.zip release/temp/pandemonium_v${version_snake_cased}_stable_javascript.zip

# Android
cp bin/android_editor.apk release/temp/pandemonium_v${version_snake_cased}_stable_android.apk

# OSX - Editor
cp -r bin/Pandemonium.app release/temp/pandemonium_v${version_snake_cased}_stable_osx.app

# ==== Copy sources ====

mkdir release/temp/pandemonium_v${version_snake_cased}_source

# Warn if a file is over a megabyte. Used to catch big temporary files that would slip through outherwise
python ./misc/scripts_app/copy_repos.py . ./release/temp/pandemonium_v${version_snake_cased}_source 1048576

# ==== Create version.txt for the export templates ====

./bin/pandemonium.x11.opt.tools.64 --version-full-config > release/temp/templates/version.txt

# ==== Zip everything ====

cd release/temp

# Export templates

zip -q -r ../pandemonium_v${version_snake_cased}_stable_export_templates.zip  ./templates/*

# Editors
# Windows (64 bit)
zip -q ../pandemonium_v${version_snake_cased}_stable_win64.zip ./pandemonium_v${version_snake_cased}_stable_win64.exe
# Windows (32 bit)
zip -q ../pandemonium_v${version_snake_cased}_stable_win32.zip ./pandemonium_v${version_snake_cased}_stable_win32.exe
# Linux (64 bit)
zip -q ../pandemonium_v${version_snake_cased}_stable_linux64.zip ./pandemonium_v${version_snake_cased}_stable_linux64
# Linux (32 bit)
zip -q ../pandemonium_v${version_snake_cased}_stable_linux32.zip ./pandemonium_v${version_snake_cased}_stable_linux32
# Pi4
zip -q ../pandemonium_v${version_snake_cased}_stable_pi4.zip ./pandemonium_v${version_snake_cased}_stable_pi4
# Server - Linux (64 bit)
zip -q ../pandemonium_v${version_snake_cased}_stable_linux_server64.zip ./pandemonium_v${version_snake_cased}_stable_linux_server64
# Android
zip -q ../pandemonium_v${version_snake_cased}_stable_android.zip ./pandemonium_v${version_snake_cased}_stable_android.apk
# OSX - Editor
zip -r -q ../pandemonium_v${version_snake_cased}_stable_osx.app.zip ./pandemonium_v${version_snake_cased}_stable_osx.app
# Sources
zip -r -q ../pandemonium_v${version_snake_cased}_source.zip ./pandemonium_v${version_snake_cased}_source

cd ..

# JS
cp ./temp/pandemonium_v${version_snake_cased}_stable_javascript.zip pandemonium_v${version_snake_cased}_stable_javascript.zip 

# ==== Rename export temlpates to tpz ====
# It's still a normal zip though
# Maybe it should be just kept as a normal zip?
mv pandemonium_v${version_snake_cased}_stable_export_templates.zip pandemonium_v${version_snake_cased}_stable_export_templates.tpz
