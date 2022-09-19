# misc-projects
Useful commands


Build Qt6 from sources with debug information:

./src/qt5/configure -prefix /opt/qt6/install -release -force-debug-info -opensource -nomake tests -no-avx -no-avx2 -no-avx512
cmake --build . --parallel
cmake --install .
