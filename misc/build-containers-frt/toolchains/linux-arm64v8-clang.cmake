set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(archOpts "-march=armv8-a")
set(ccOpts "-target aarch64-linux-gnu")
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_FLAGS "${archOpts} ${ccOpts}")
set(CMAKE_CXX_FLAGS "${archOpts} ${ccOpts}")

