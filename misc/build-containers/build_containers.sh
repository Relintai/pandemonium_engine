#!/bin/bash
set -e

podman=`which podman || true`

if [ -z $podman ]; then
  echo "podman needs to be in PATH for this script to work."
  exit 1
fi

files_root=$(pwd)/files
img_version=pe

mkdir -p logs

# You can add --no-cache  as an option to podman_build below to rebuild all containers from scratch
export podman_build="$podman build --build-arg img_version=${img_version} -v ${files_root}:/root/files"

$podman build -t pandemonium-fedora:${img_version} -f Dockerfile.base . 2>&1 | tee logs/base.log
$podman_build -t pandemonium-linux:${img_version} -f Dockerfile.linux . 2>&1 | tee logs/linux.log
$podman_build -t pandemonium-windows:${img_version} -f Dockerfile.windows . 2>&1 | tee logs/windows.log
$podman_build -t pandemonium-javascript:${img_version} -f Dockerfile.javascript . 2>&1 | tee logs/javascript.log
$podman_build -t pandemonium-android:${img_version} -f Dockerfile.android . 2>&1 | tee logs/android.log

XCODE_SDK=13.3.1
OSX_SDK=12.3
IOS_SDK=15.4
if [ ! -e files/MacOSX${OSX_SDK}.sdk.tar.xz ] || [ ! -e files/iPhoneOS${IOS_SDK}.sdk.tar.xz ] || [ ! -e files/iPhoneSimulator${IOS_SDK}.sdk.tar.xz ]; then
  if [ ! -e files/Xcode_${XCODE_SDK}.xip ]; then
    echo "files/Xcode_${XCODE_SDK}.xip is required. It can be downloaded from https://developer.apple.com/download/more/ with a valid apple ID."
    exit 1
  fi

  echo "Building OSX and iOS SDK packages. This will take a while"
  $podman_build -t pandemonium-xcode-packer:${img_version} -f Dockerfile.xcode . 2>&1 | tee logs/xcode.log
  $podman run -it --rm -v ${files_root}:/root/files -e XCODE_SDKV="${XCODE_SDK}" -e OSX_SDKV="${OSX_SDK}" -e IOS_SDKV="${IOS_SDK}" pandemonium-xcode-packer:${img_version} 2>&1 | tee logs/xcode_packer.log
fi

$podman_build -t pandemonium-osx:${img_version} -f Dockerfile.osx . 2>&1 | tee logs/osx.log
$podman_build -t pandemonium-ios:${img_version} -f Dockerfile.ios . 2>&1 | tee logs/ios.log

