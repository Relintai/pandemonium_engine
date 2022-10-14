cd ../../bin/

files=(
  # Windows (64 bit)
  "pandemonium.windows.opt.64.exe"
  "pandemonium.windows.opt.debug.64.exe"
  "pandemonium.windows.opt.tools.64.exe"

  # Windows (32 bit)
  "pandemonium.windows.opt.32.exe"
  "pandemonium.windows.opt.debug.32.exe"
  "pandemonium.windows.opt.tools.32.exe"
  
  # Linux (64 bit)
  "pandemonium.x11.opt.64"
  "pandemonium.x11.opt.debug.64"
  "pandemonium.x11.opt.tools.64"

  # Pi4
  "pandemonium.x11.opt.pi4"
  "pandemonium.x11.opt.debug.pi4"
  "pandemonium.x11.opt.tools.pi4"

  # Server - Linux (64 bit)
  "pandemonium_server.x11.opt.64"
  "pandemonium_server.x11.opt.debug.64"
  "pandemonium_server.x11.opt.tools.64"

  # Linux (32 bit)
  "pandemonium.x11.opt.64"
  "pandemonium.x11.opt.debug.64"
  "pandemonium.x11.opt.tools.64"

  # JS
  "pandemonium.javascript.opt.zip"
  "pandemonium.javascript.opt.debug.zip"

  "pandemonium.javascript.opt.threads.zip"
  "pandemonium.javascript.opt.debug.threads.zip"

  "pandemonium.javascript.opt.tools.threads.zip"

  # Android
  "android_source.zip"
  "android_debug.apk"
  "android_release.apk"
  "android_editor.apk"
  "pandemonium-lib.release.aar"

  # OSX - Editor
  "Pandemonium.app.zip"

  # OSX - export templates
  "osx.zip"
)

error=0

for f in ${files[*]} 
do
if [ ! -e $f ]; then
  error=1
  echo "$f is not present!"
fi
done

if [ $error -eq 0 ]; then
  echo "All files are present!"
fi

cd ../..
