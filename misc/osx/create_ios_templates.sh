cd ../../bin/

rm -Rf ios_xcode
rm -f iphone.zip

cp -r ../misc/dist/ios_xcode ios_xcode

cp libpandemonium.iphone.opt.arm64.a ios_xcode/libpandemonium.iphone.release.xcframework/ios-arm64/libpandemonium.a
cp libpandemonium.iphone.opt.x86_64.simulator.a ios_xcode/libpandemonium.iphone.release.xcframework/ios-arm64_x86_64-simulator/libpandemonium.a

cp libpandemonium.iphone.opt.debug.arm64.a ios_xcode/libpandemonium.iphone.debug.xcframework/ios-arm64/libpandemonium.a
cp libpandemonium.iphone.opt.debug.x86_64.simulator.a ios_xcode/libpandemonium.iphone.debug.xcframework/ios-arm64_x86_64-simulator/libpandemonium.a

cd ios_xcode

zip -q -r ../iphone.zip  *
