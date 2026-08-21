#!/bin/sh
SKIP_LIST="./.git,./bin,./thirdparty,*.gen.*,*.po,*.pot,package-lock.json,./core/string/locales.h,./core/locales.h,./DONORS.md,./misc/dist/linux/org.pandemoniumengine.pandemonium.Pandemonium.desktop,./misc/scripts/codespell.sh,./modules/database_sqlite/sqlite/*,./modules/lz4/thirdparty/*,./modules/web/html/libs/*,./modules/mesh_utils/xatlas/*,./modules/mesh_utils/delaunay/*,./editor_modules/denoise/oidn/*,./editor_modules/fbx/data/fbx_material.cpp,./editor_modules/raycast/embree/*,./modules/pdf_generator/libharu/*,./modules/procedural_tree_3d/proctree/*,./modules/xatlas_unwrap/xatlas/*,./modules/webp/thirdparty/libwebp/*,./AUTHORS.md"
IGNORE_LIST="ba,childs,commiting,complies,curvelinear,doubleclick,expct,fave,findn,gird,inout,leapyear,lod,nd,numer,ois,readded,ro,statics,switchs,te,varius,varn,translater,acount,matc,ede,indx,thisY,ALPH,breaked,coo,requestor"

codespell -w -q 3 -S "${SKIP_LIST}" -L "${IGNORE_LIST}"
