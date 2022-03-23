#!/bin/bash
set -e

podman=`which podman || true`

if [ -z $podman ]; then
  echo "podman needs to be in PATH for this script to work."
  exit 1
fi

cd ../..

project_root=$(pwd)/
img_version=pe

mkdir -p logs

rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-windows:${img_version} scons tools=yes target=release_debug debug_symbols=no platform=windows -j4 . 2>&1 | tee logs/bew.log
rm -f modules/modules_enabled.gen.h
#$podman run -v ${project_root}:/root/project -w /root/project pandemonium-windows:${img_version} scons tools=yes target=debug custom_modules_shared=no debug_symbols=no platform=windows -j4 . 2>&1 | tee logs/bewd.log
rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-windows:${img_version} scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=windows -j4 . 2>&1 | tee logs/bw.log
rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-windows:${img_version} scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=windows -j4 . 2>&1 | tee logs/bwr.log
rm -f modules/modules_enabled.gen.h


$podman run -v ${project_root}:/root/project -w /root/project pandemonium-linux:${img_version} scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 -j4 . 2>&1 | tee logs/bel.log
rm -f modules/modules_enabled.gen.h
#$podman run -v ${project_root}:/root/project -w /root/project pandemonium-linux:${img_version} scons tools=yes target=debug custom_modules_shared=no debug_symbols=no platform=x11 -j4 . 2>&1 | tee logs/beld.log
rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-linux:${img_version} scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 -j4 . 2>&1 | tee logs/bl.log
rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-linux:${img_version} scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=x11 -j4 . 2>&1 | tee logs/blr.log
rm -f modules/modules_enabled.gen.h


$podman run -v ${project_root}:/root/project -w /root/project pandemonium-javascript:${img_version} bash -c 'source /root/emsdk_2.0.25/emsdk_env.sh;scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=javascript -j4' . 2>&1 | tee logs/bj.log
rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-javascript:${img_version} bash -c 'source /root/emsdk_2.0.25/emsdk_env.sh;scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=javascript -j4' . 2>&1 | tee logs/bjr.log
rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-javascript:${img_version} bash -c 'source /root/emsdk_2.0.25/emsdk_env.sh;scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no threads_enabled=yes platform=javascript -j4' . 2>&1 | tee logs/bej.log
rm -f modules/modules_enabled.gen.h

$podman run -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=armv7 -j4 . 2>&1 | tee logs/ba.log
rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=arm64v8 -j4 . 2>&1 | tee logs/ba.log
rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=x86 -j4 . 2>&1 | tee logs/ba.log
rm -f modules/modules_enabled.gen.h

cd platform/android/java/
./gradlew generateGodotTemplates
cd ../../..

$podman run -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=android android_arch=armv7 -j4 . 2>&1 | tee logs/bar.log
rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=android android_arch=arm64v8 -j4 . 2>&1 | tee logs/bar.log
rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=android android_arch=x86 -j4 . 2>&1 | tee logs/bar.log
rm -f modules/modules_enabled.gen.h

cd platform/android/java/
./gradlew generateGodotTemplates
cd ../../..

$podman run -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=armv7 -j4 . 2>&1 | tee logs/bae.log
rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=arm64v8 -j4 . 2>&1 | tee logs/bae.log
rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=x86 -j4 . 2>&1 | tee logs/bae.log
rm -f modules/modules_enabled.gen.h

cd platform/android/java/
./gradlew generateGodotEditor
cd ../../..

#osx
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-osx:${img_version} scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=osx arch=x86_64 -j4 osxcross_sdk=darwin20.4 . 2>&1 | tee logs/bex_x86_64.log
rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-osx:${img_version} scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=osx arch=arm64 -j4 osxcross_sdk=darwin20.4 . 2>&1 | tee logs/bex_arm64.log
rm -f modules/modules_enabled.gen.h

$podman run -v ${project_root}:/root/project -w /root/project pandemonium-osx:${img_version} scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=osx arch=x86_64 -j4 osxcross_sdk=darwin20.4 . 2>&1 | tee logs/bx_x86_64.log
rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-osx:${img_version} scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=osx arch=arm64 -j4 osxcross_sdk=darwin20.4 . 2>&1 | tee logs/bx_arm64.log
rm -f modules/modules_enabled.gen.h

$podman run -v ${project_root}:/root/project -w /root/project pandemonium-osx:${img_version} scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=osx arch=x86_64 -j4 osxcross_sdk=darwin20.4 . 2>&1 | tee logs/bxr_x86_64.log
rm -f modules/modules_enabled.gen.h
$podman run -v ${project_root}:/root/project -w /root/project pandemonium-osx:${img_version} scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=osx arch=arm64 -j4 osxcross_sdk=darwin20.4 . 2>&1 | tee logs/bxr_arm64.log
rm -f modules/modules_enabled.gen.h

#lipo
$podman run -v ${project_root}:/root/project -w /root/project/tools/osx pandemonium-osx:${img_version} bash -c ./lipo.sh

#ios
#$podman run -v ${project_root}:/root/project -w /root/project pandemonium-ios:${img_version} scons bir_strip -j4 . 2>&1 | tee logs/bir.log
#rm -f modules/modules_enabled.gen.h

# $podman run -v ${project_root}:/root/project -i -w /root/project -t pandemonium-windows:${img_version} scons bew -j4
rm -f modules/modules_enabled.gen.h

# Check files

cd ./bin/

files=(
  # Windows
  "pandemonium.windows.opt.64.exe"
  "pandemonium.windows.opt.debug.64.exe"
  "pandemonium.windows.opt.tools.64.exe"
  # Linux
  "pandemonium.x11.opt.64"
  "pandemonium.x11.opt.debug.64"
  "pandemonium.x11.opt.tools.64"
  # JS
  "pandemonium.javascript.opt.tools.threads.zip"
  "pandemonium.javascript.opt.zip"
  # Android
  "android_debug.apk"
  "android_release.apk"
  "android_editor.apk"
  # OSX
  "pandemonium.osx.opt.arm64"
  "pandemonium.osx.opt.debug.arm64"
  "pandemonium.osx.opt.debug.universal"
  "pandemonium.osx.opt.debug.x86_64"
  "pandemonium.osx.opt.tools.arm64"
  "pandemonium.osx.opt.tools.universal"
  "pandemonium.osx.opt.tools.x86_64"
  "pandemonium.osx.opt.universal"
  "pandemonium.osx.opt.x86_64"
)

error=0

for f in ${files[*]} 
do
if [ ! -e $f ]; then
  error=1
  echo "$f is not present!"
fi
done

if [ $error -eq 0 ]; then
  echo "All files are present!"
fi

cd ../..