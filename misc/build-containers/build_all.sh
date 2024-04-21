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
#docker run ${custom_envvars} \
#        -v ${basedir}/engine_build_scripts:/root/engine_build_scripts \
#        -v ${project_root}:/root/project \
#        -w /root/project pandemonium-javascript:${img_version} \
#        bash /root/engine_build_scripts/javascript.sh "$@" 2>&1 | tee logs/javascript.log


# Android Build
#docker run ${custom_envvars} \
#       -v ${basedir}/engine_build_scripts:/root/engine_build_scripts \
#       -v ${project_root}:/root/project \
#       -w /root/project pandemonium-android:${img_version} \
#       bash /root/engine_build_scripts/android.sh "$@" 2>&1 | tee logs/android.log
 
# OSX Build
#docker run ${custom_envvars} \
#       -v ${basedir}/engine_build_scripts:/root/engine_build_scripts \
#       -v ${project_root}:/root/project \
#       -w /root/project pandemonium-osx:${img_version} \
#       bash /root/engine_build_scripts/osx.sh "$@" 2>&1 | tee logs/osx.log


# iOS Build
#docker run ${custom_envvars} \
#       -v ${basedir}/engine_build_scripts:/root/engine_build_scripts \
#       -v ${project_root}:/root/project \
#       -w /root/project pandemonium-ios:${img_version} \
#       bash /root/engine_build_scripts/ios.sh "$@" 2>&1 | tee logs/ios.log


# FRT Build arm64
#docker run ${custom_envvars} \
#        -v ${basedir}/engine_build_scripts:/root/engine_build_scripts \
#        -v ${project_root}:/root/project \
#        -w /root/project pandemonium-frt-arm64v8:${img_version} \
#        bash /root/engine_build_scripts/frt_arm64.sh "$@" 2>&1 | tee logs/frt_arm64.log

# FRT Build arm32
docker run ${custom_envvars} \
        -v ${basedir}/engine_build_scripts:/root/engine_build_scripts \
        -v ${project_root}:/root/project \
        -w /root/project pandemonium-frt-arm32v7:${img_version} \
        bash /root/engine_build_scripts/frt_arm32.sh "$@" 2>&1 | tee logs/frt_arm32.log

# Check files

exit 1

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
  "pandemonium-lib.debug.aar"

  # OSX
  "pandemonium.osx.opt.universal"
  "pandemonium.osx.opt.debug.universal"
  "pandemonium.osx.opt.tools.universal"

  # Note: These are not needed in the final release, only the universals.
  "pandemonium.osx.opt.arm64"
  "pandemonium.osx.opt.x86_64"

  "pandemonium.osx.opt.debug.arm64"
  "pandemonium.osx.opt.debug.x86_64"

  "pandemonium.osx.opt.tools.arm64"
  "pandemonium.osx.opt.tools.x86_64"

  # iOS
  "libpandemonium.iphone.opt.arm64.a"
  "libpandemonium.iphone.opt.debug.arm64.a"

  "libpandemonium.iphone.opt.debug.x86_64.simulator.a"
  "libpandemonium.iphone.opt.x86_64.simulator.a"
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
