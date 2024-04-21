export STRIP="x86_64-apple-darwin23.3-strip -u -r" 

# Tools
lipo -create  ../../bin/pandemonium.osx.opt.tools.x86_64 ../../bin/pandemonium.osx.opt.tools.arm64 -output ../../bin/pandemonium.osx.opt.tools.universal
$STRIP ../../bin/pandemonium.osx.opt.tools.universal

# Export Templates
lipo -create  ../../bin/pandemonium.osx.opt.x86_64 ../../bin/pandemonium.osx.opt.arm64 -output ../../bin/pandemonium.osx.opt.universal
$STRIP ../../bin/pandemonium.osx.opt.universal

lipo -create  ../../bin/pandemonium.osx.opt.debug.x86_64 ../../bin/pandemonium.osx.opt.debug.arm64 -output ../../bin/pandemonium.osx.opt.debug.universal
$STRIP ../../bin/pandemonium.osx.opt.debug.universal
