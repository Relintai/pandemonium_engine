cd ../../bin/

rm -Rf Pandemonium.app
rm -f Pandemonium.app.zip

cp -r ../misc/dist/osx_tools.app Pandemonium.app
mkdir -p Pandemonium.app/Contents/MacOS
cp pandemonium.osx.opt.tools.universal Pandemonium.app/Contents/MacOS/Pandemonium
chmod +x Pandemonium.app/Contents/MacOS/Pandemonium

zip -q -r Pandemonium.app.zip  Pandemonium.app/*
