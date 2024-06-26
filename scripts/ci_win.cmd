call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\VC\\Auxiliary\\Build\\vcvarsall.bat" x86_x64

cmake .. ^
    -G Ninja ^
    -DCMAKE_C_COMPILER_LAUNCHER=ccache ^
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache ^
    -DCMAKE_TOOLCHAIN_FILE=..\vcpkg\scripts\buildsystems\vcpkg.cmake ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DMSGPACK_LIGHT_TESTING:BOOL=ON ^
    -DMSGPACK_LIGHT_ENABLE_UNIT_TESTS=ON ^
    -DMSGPACK_LIGHT_ENABLE_INTEG_TESTS=ON ^
    -DMSGPACK_LIGHT_ENABLE_BENCH=OFF ^
    -DMSGPACK_LIGHT_TEST_BENCHMARKS=OFF ^
    -DMSGPACK_LIGHT_BUILD_EXAMPLES=OFF ^
    -DMSGPACK_LIGHT_WRITE_JUNIT:BOOL=ON ^
    -DBUILD_SHARED_LIBS:BOOL=ON

cmake --build . --config Release --parallel

ctest -V --build-config Release
