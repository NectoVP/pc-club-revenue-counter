# clone:

`
git clone git@github.com:NectoVP/pc-club-revenue-counter.git
`

# navigate inside

`
cd pc-club-revenue-counter
`

# To build (for Unix) write:

`
/usr/bin/cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ --no-warn-unused-cli -S./ -B./build -G "Unix Makefiles"
`

`
/usr/bin/cmake --build ./build --config Debug --target all -j 18 --
`

# NOTE: make sure to use correct path for CMake, gcc compier and g++ compiler

# Run

./build/PcClubCounter file.txt

# Run tests

./build/tests/tests