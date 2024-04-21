#!/bin/bash
set -e

basedir=$(pwd)/

cd ../..

project_root=$(pwd)/
img_version=pe

mkdir -p logs

custom_envvars=''

if [[ -z "${OFFICIAL}" ]]; then
  custom_envvars='-e BUILD_NAME=official'
fi

# Windows Build
#docker run ${custom_envvars} \
#	-v ${basedir}/engine_build_scripts:/root/engine_build_scripts \
#	-v ${project_root}:/root/project \
#	-w /root/project pandemonium-windows:${img_version} \
#	bash /root/engine_build_scripts/windows.sh "$@" 2>&1 | tee logs/windows.log

# Linux Build
#docker run ${custom_envvars} \
#        -v ${basedir}/engine_build_scripts:/root/engine_build_scripts \
#        -v ${project_root}:/root/project \
#        -w /root/project pandemonium-linux:${img_version} \
#        bash /root/engine_build_scripts/linux.sh "$@" 2>&1 | tee logs/linux.log


# Javascript Build
docker run ${custom_envvars} \
        -v ${basedir}/engine_build_scripts:/root/engine_build_scripts \
        -v ${project_root}:/root/project \
        -w /root/project pandemonium-javascript:${img_version} \
        bash /root/engine_build_scripts/javascript.sh "$@" 2>&1 | tee logs/javascript.log

exit 1

# Android editor
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=armv7 "$@" . 2>&1 | tee logs/android_editor_armv7.log
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=arm64v8 "$@" . 2>&1 | tee logs/android_editor_arm64v8.log
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=x86 "$@" . 2>&1 | tee logs/android_editor_x86.log
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=x86_64 "$@" . 2>&1 | tee logs/android_editor_x86_64.log

docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} bash -c 'cd platform/android/java/;./gradlew generatePandemoniumEditor' . 2>&1 | tee logs/android_editor_assemble.log

# Android templates release_debug
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=armv7 "$@" . 2>&1 | tee logs/android_template_rd_armv7.log
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=arm64v8 "$@" . 2>&1 | tee logs/android_template_rd_arm64v8.log
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=x86 "$@" . 2>&1 | tee logs/android_template_rd_x86.log
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=android android_arch=x86_64 "$@" . 2>&1 | tee logs/android_template_rd_x86_64.log

docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} bash -c 'cd platform/android/java/;./gradlew generatePandemoniumTemplates' . 2>&1 | tee logs/android_template_rd_assemble.log

# Android templates release
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=android android_arch=armv7 "$@" . 2>&1 | tee logs/android_template_r_armv7.log
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=android android_arch=arm64v8 "$@" . 2>&1 | tee logs/android_template_r_arm64v8.log
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=android android_arch=x86 "$@" . 2>&1 | tee logs/android_template_r_x86.log
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=android android_arch=x86_64 "$@" . 2>&1 | tee logs/android_template_r_x86_64.log

docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-android:${img_version} bash -c 'cd platform/android/java/;./gradlew generatePandemoniumTemplates' . 2>&1 | tee logs/android_template_r_assemble.log

# OSX editor
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-osx:${img_version} scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=osx arch=x86_64 "$@" osxcross_sdk=darwin21.4 . 2>&1 | tee logs/osx_editor_x86_64.log
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-osx:${img_version} scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=osx arch=arm64 "$@" osxcross_sdk=darwin21.4 . 2>&1 | tee logs/sx_editor_arm64.log

# OSX templates release_debug
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-osx:${img_version} scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=osx arch=x86_64 "$@" osxcross_sdk=darwin21.4 . 2>&1 | tee logs/osx_template_rd_x86_64.log
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-osx:${img_version} scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=osx arch=arm64 "$@" osxcross_sdk=darwin21.4 . 2>&1 | tee logs/osx_template_rd_arm64.log

# OSX templates release
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-osx:${img_version} scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=osx arch=x86_64 "$@" osxcross_sdk=darwin21.4 . 2>&1 | tee logs/osx_template_r_x86_64.log
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-osx:${img_version} scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=osx arch=arm64 "$@" osxcross_sdk=darwin21.4 . 2>&1 | tee logs/osx_template_r_arm64.log

# OSX lipo
docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project/misc/osx pandemonium-osx:${img_version} bash -c ./lipo.sh

#ios
#docker run ${custom_envvars} -v ${project_root}:/root/project -w /root/project pandemonium-ios:${img_version} scons bir_strip "$@" . 2>&1 | tee logs/bir.log

# docker run ${custom_envvars} -v ${project_root}:/root/project -i -w /root/project -t pandemonium-windows:${img_version} scons bew "$@"

# Check files

cd ./bin/

files=(
  # Windows
  "pandemonium.windows.opt.64.exe"
  "pandemonium.windows.opt.debug.64.exe"
  
  "pandemonium.windows.opt.32.exe"
  "pandemonium.windows.opt.debug.32.exe"

  "pandemonium.windows.opt.tools.64.exe"
  "pandemonium.windows.opt.tools.32.exe"

  # Linux
  "pandemonium.x11.opt.32"
  "pandemonium.x11.opt.64"
  "pandemonium.x11.opt.arm"
  "pandemonium.x11.opt.arm64"

  "pandemonium.x11.opt.debug.32"
  "pandemonium.x11.opt.debug.64"
  "pandemonium.x11.opt.debug.arm"
  "pandemonium.x11.opt.debug.arm64"

  "pandemonium.x11.opt.tools.32"
  "pandemonium.x11.opt.tools.64"
  "pandemonium.x11.opt.tools.arm"
  "pandemonium.x11.opt.tools.arm64"

  # Server (Linux) - template
  "pandemonium_server.x11.opt.64"
  "pandemonium_server.x11.opt.debug.64"

  # Headless (Linux) - editor

  "pandemonium_server.x11.opt.tools.64"

  # JS
  "pandemonium.javascript.opt.zip"
  "pandemonium.javascript.opt.debug.zip"

  "pandemonium.javascript.opt.threads.zip"
  "pandemonium.javascript.opt.debug.threads.zip"

  "pandemonium.javascript.opt.tools.threads.zip"

  "pandemonium.javascript.opt.gdnative.zip"
  "pandemonium.javascript.opt.debug.gdnative.zip"

  # Android
  "android_source.zip"
  "android_debug.apk"
  "android_release.apk"
  "android_editor.apk"
  "pandemonium-lib.release.aar"

  # OSX
  "pandemonium.osx.opt.arm64"
  "pandemonium.osx.opt.universal"
  "pandemonium.osx.opt.x86_64"

  "pandemonium.osx.opt.debug.arm64"
  "pandemonium.osx.opt.debug.universal"
  "pandemonium.osx.opt.debug.x86_64"

  "pandemonium.osx.opt.tools.arm64"
  "pandemonium.osx.opt.tools.universal"
  "pandemonium.osx.opt.tools.x86_64"
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

cd ..
