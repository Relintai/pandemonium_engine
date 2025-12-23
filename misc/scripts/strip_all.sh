#!/bin/bash

cd ../../bin/

# Todo figure out a way to also strip the pi4 versions (from x86 linux)

strip -v -s pandemonium.x11.*
strip -v -s pandemonium_server.x11.*
strip -v -s pandemonium.windows.*
strip -v -s pandemonium_server.*
strip -v -s pandemonium_http_server.*

