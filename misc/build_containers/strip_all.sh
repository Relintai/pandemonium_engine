#!/bin/bash
set -e

basedir=$(pwd)/

cd ../..

project_root=$(pwd)/
img_version=pe

mkdir -p logs

custom_envvars=''

docker run ${custom_envvars} \
        -v ${basedir}/engine_build_scripts:/root/engine_build_scripts \
        -v ${project_root}:/root/project \
        -w /root/project pandemonium-linux:${img_version} \
        bash /root/engine_build_scripts/strip_all.sh "$@" 2>&1 | tee logs/strip_all.log

