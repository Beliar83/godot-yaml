cmake .. -G "%cmake_generator%" -A "%cmake_platform%" -DCMAKE_INSTALL_PREFIX="../../rapidyaml-install" -DRYML_BUILD_TOOLS="OFF" -Wno-dev
nmake
nmake install