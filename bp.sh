#!/bin/bash

rm -f bp.patch

cd ../godot

rm -f bp.patch

git format-patch -1 "$1" --stdout > bp.patch

mv bp.patch ../pandemonium_engine_backport/

git am --ignore-whitespace --ignore-space-change -3 < bp.patch


