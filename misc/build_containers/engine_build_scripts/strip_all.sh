#!/bin/bash
#set -e

cd bin

# TODO only strip the release template binaries,
# the release debug ones should stay non stripped.
# This should improve their usefulness, and also differentiate them a bit more.

export PATH="${PANDEMONIUN_SDK_LINUX_ARMV6}/bin:${BASE_PATH}"

strip -v -s pandemonium.x11.*
strip -v -s pandemonium_server.x11.*
strip -v -s pandemonium.windows.*
strip -v -s pandemonium_server.*
strip -v -s pandemonium_http_server.*

export PATH="${PANDEMONIUN_SDK_LINUX_ARMV7}/bin:${BASE_PATH}"

strip -v -s pandemonium.x11.*
strip -v -s pandemonium_server.x11.*
strip -v -s pandemonium.windows.*
strip -v -s pandemonium_server.*
strip -v -s pandemonium_http_server.*

export PATH="${PANDEMONIUN_SDK_LINUX_ARM64}/bin:${BASE_PATH}"

strip -v -s pandemonium.x11.*
strip -v -s pandemonium_server.x11.*
strip -v -s pandemonium.windows.*
strip -v -s pandemonium_server.*
strip -v -s pandemonium_http_server.*

export PATH="${PANDEMONIUN_SDK_LINUX_X86_64}/bin:${BASE_PATH}"

strip -v -s pandemonium.x11.*
strip -v -s pandemonium_server.x11.*
strip -v -s pandemonium.windows.*
strip -v -s pandemonium_server.*
strip -v -s pandemonium_http_server.*

export PATH="${PANDEMONIUN_SDK_LINUX_X86_32}/bin:${BASE_PATH}"

strip -v -s pandemonium.x11.*
strip -v -s pandemonium_server.x11.*
strip -v -s pandemonium.windows.*
strip -v -s pandemonium_server.*
strip -v -s pandemonium_http_server.*

