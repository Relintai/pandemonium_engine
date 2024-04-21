set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(archOpts "-march=armv8-a")
set(ccOpts "")
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_C_FLAGS "${archOpts} ${ccOpts}")
set(CMAKE_CXX_FLAGS "${archOpts} ${ccOpts}")
