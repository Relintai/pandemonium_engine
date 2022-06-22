#!/bin/bash
set -e

full_project_name=$1
slim_project_name=$2

version=""
version_snake_cased=""

if [ ! -z $3 ]; then
    version="."
    version+=$3

    version_snake_cased=${version//./_}
fi

project_root=$(pwd)

rm -Rf ./release

mkdir release

cd export

rm -Rf ${full_project_name}${version_snake_cased}_full_source
rm -Rf ${full_project_name}${version_snake_cased}_game_source

mkdir ${full_project_name}${version_snake_cased}_full_source
mkdir ${full_project_name}${version_snake_cased}_game_source

# Warn if a file is over a megabyte. Used to catch big temporary files that would slip through outherwise
python ../tools/copy_repos.py ../ ./${full_project_name}${version_snake_cased}_full_source 1048576
python ../tools/copy_repos.py ../game/ ./${full_project_name}${version_snake_cased}_game_source

zip -q ../release/${full_project_name}${version_snake_cased}_android_debug.zip  ./${full_project_name}${version_snake_cased}_android_debug/*
zip -q ../release/${full_project_name}${version_snake_cased}_android_release.zip  ./${full_project_name}${version_snake_cased}_android_release/*
zip -q ../release/${full_project_name}${version_snake_cased}_javascript.zip  ./${full_project_name}${version_snake_cased}_javascript/*
zip -q ../release/${full_project_name}${version_snake_cased}_linux.zip  ./${full_project_name}${version_snake_cased}_linux/*
zip -q ../release/${full_project_name}${version_snake_cased}_windows.zip  ./${full_project_name}${version_snake_cased}_windows/*
zip -q ../release/${full_project_name}${version_snake_cased}_pi4.zip  ./${full_project_name}${version_snake_cased}_pi4/*
zip -r -q ../release/${full_project_name}${version_snake_cased}_osx.zip  ./${full_project_name}${version_snake_cased}_osx/*

# Editor
zip -q ../release/editor_windows_${slim_project_name}${version_snake_cased}.zip  ./pandemonium.${slim_project_name}${version}.windows.opt.tools.64.exe
zip -q ../release/editor_linux_${slim_project_name}${version_snake_cased}.zip  ./pandemonium.${slim_project_name}${version}.x11.opt.tools.64
zip -q ../release/editor_pi4_${slim_project_name}${version_snake_cased}.zip  ./pandemonium.${slim_project_name}${version}.x11.pi4.opt.tools.32
cp  ./pandemonium.${slim_project_name}${version}.javascript.opt.tools.zip ../release/editor_javascript_${slim_project_name}${version_snake_cased}.zip
zip -q ../release/editor_osx_${slim_project_name}${version_snake_cased}.zip  ./pandemonium.${slim_project_name}${version}.osx.opt.tools.zip
zip -q ../release/pandemonium.${slim_project_name}${version}.android_editor.zip  ./pandemonium.${slim_project_name}${version}.android_editor.apk

zip -q ../release/export_templates_${slim_project_name}${version_snake_cased}.zip  ./export_templates_${slim_project_name}${version_snake_cased}/*
#mv ../release/export_templates_${slim_project_name}${version_snake_cased}.zip ../release/export_templates_${slim_project_name}${version_snake_cased}.tpz

zip -q -r ../release/${full_project_name}${version_snake_cased}_full_source.zip  ./${full_project_name}${version_snake_cased}_full_source/*
zip -q -r ../release/${full_project_name}${version_snake_cased}_game_source.zip  ./${full_project_name}${version_snake_cased}_game_source/*

cd ..

