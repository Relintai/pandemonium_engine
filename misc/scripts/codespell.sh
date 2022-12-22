#!/bin/sh
SKIP_LIST="./.git,./bin,./thirdparty,*.gen.*,*.po,*.pot,package-lock.json,./core/string/locales.h,./DONORS.md,./misc/dist/linux/net.relintai.pandemonium.Pandemonium.desktop,./misc/scripts/codespell.sh,./modules/database_sqlite/sqlite/*,./modules/lz4/thirdparty/*,./modules/web/html/libs/*,./modules/mesh_utils/xatlas/*,./modules/mesh_utils/delaunay/*"
IGNORE_LIST="ba,childs,commiting,complies,curvelinear,doubleclick,expct,fave,findn,gird,inout,leapyear,lod,nd,numer,ois,readded,ro,statics,switchs,te,varius,varn,translater,acount,matc,ede"

codespell -w -q 3 -S "${SKIP_LIST}" -L "${IGNORE_LIST}"
