set -e

cmake -S . -B Build
cmake --build Build
cd Build
ctest --output-on-failure
cd -
