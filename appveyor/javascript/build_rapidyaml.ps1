emcmake cmake .. -G "$cmake_generator" -DCMAKE_INSTALL_PREFIX="../../rapidyaml-install" -DRYML_BUILD_TOOLS="OFF" -DCMAKE_BUILD_TYPE="$CONFIGURATION" -DCMAKE_POSITION_INDEPENDENT_CODE="ON" -Wno-dev
make -j$(nproc)
make install