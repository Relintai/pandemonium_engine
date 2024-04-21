#! /bin/sh
set -e

[ -f Dockerfile.$1 ]

exec docker build -t crossbuild:$1 -f Dockerfile.$1 .
