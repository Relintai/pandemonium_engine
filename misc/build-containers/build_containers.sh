#!/bin/bash
set -e

files_root=$(pwd)/files
img_version=pe

mkdir -p logs

# You can add --no-cache  as an option to docker_build below to rebuild all containers from scratch
export docker_build="docker build --build-arg img_version=${img_version} "

docker build -t pandemonium-fedora:${img_version} -f Dockerfile.base . 2>&1 | tee logs/base.log
$docker_build -t pandemonium-linux:${img_version} -f Dockerfile.linux . 2>&1 | tee logs/linux.log
$docker_build -t pandemonium-windows:${img_version} -f Dockerfile.windows . 2>&1 | tee logs/windows.log
$docker_build -t pandemonium-javascript:${img_version} -f Dockerfile.javascript . 2>&1 | tee logs/javascript.log
$docker_build -t pandemonium-android:${img_version} -f Dockerfile.android . 2>&1 | tee logs/android.log

XCODE_SDK=15.2
OSX_SDK=14.2
IOS_SDK=17.2
if [ ! -e files/MacOSX${OSX_SDK}.sdk.tar.xz ] || [ ! -e files/iPhoneOS${IOS_SDK}.sdk.tar.xz ] || [ ! -e files/iPhoneSimulator${IOS_SDK}.sdk.tar.xz ]; then
  if [ ! -e files/Xcode_${XCODE_SDK}.xip ]; then
    echo "files/Xcode_${XCODE_SDK}.xip is required. It can be downloaded from https://developer.apple.com/download/more/ with a valid apple ID."
    exit 1
  fi

  echo "Building OSX and iOS SDK packages. This will take a while"
  $docker_build -t pandemonium-xcode-packer:${img_version} -f Dockerfile.xcode . 2>&1 | tee logs/xcode.log
  docker run -it --rm -v ${files_root}:/root/files -e XCODE_SDKV="${XCODE_SDK}" -e OSX_SDKV="${OSX_SDK}" -e IOS_SDKV="${IOS_SDK}" pandemonium-xcode-packer:${img_version} 2>&1 | tee logs/xcode_packer.log
fi

$docker_build -t pandemonium-osx:${img_version} -f Dockerfile.osx . 2>&1 | tee logs/osx.log
$docker_build -t pandemonium-ios:${img_version} -f Dockerfile.ios . 2>&1 | tee logs/ios.log

docker build -t pandemonium-frt-base:${img_version} -f Dockerfile.frt_base . 2>&1 | tee logs/frt_base.log
$docker_build -t pandemonium-frt-arm32:${img_version} -f Dockerfile.frt_arm32 . 2>&1 | tee logs/frt_arm32.log
$docker_build -t pandemonium-frt-arm64:${img_version} -f Dockerfile.frt_arm64 . 2>&1 | tee logs/frt_arm64.log

