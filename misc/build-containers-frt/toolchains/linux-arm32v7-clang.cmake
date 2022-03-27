set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(archOpts "-march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard")
set(ccOpts "-target arm-linux-gnueabihf")
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_FLAGS "${archOpts} ${ccOpts}")
set(CMAKE_CXX_FLAGS "${archOpts} ${ccOpts}")

