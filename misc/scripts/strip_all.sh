#!/bin/bash

cd ../../bin/


strip -v -s pandemonium.x11.*
strip -v -s pandemonium_server.x11.*
strip -v -s pandemonium.windows.*
strip -v -s pandemonium_server.*
strip -v -s pandemonium_http_server.*

# sudo pacman -S aarch64-linux-gnu-binutils

aarch64-linux-gnu-strip -v -s pandemonium.x11.*
aarch64-linux-gnu-strip -v -s pandemonium_server.*
aarch64-linux-gnu-strip -v -s pandemonium_http_server.*
aarch64-linux-gnu-strip -v -s pandemonium.frt.*
aarch64-linux-gnu-strip -v -s pandemonium.frt_sdl.*

