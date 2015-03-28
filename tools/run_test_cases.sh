# ! /bin/bash

test_files=test_*
if [ x$1 != x ]; then
    test_files=$1
fi


if [ -f "_test.exe" ]; then
rm  _test.exe
fi

if [ -f "*.module" ]; then
rm  *.module
fi


gcc	tests/dummy_module.cpp		-shared	-o dummy.module	-D NDEBUG	-lstdc++
gcc	src/simple/*.cpp    src/simple/*.c  tests/main.cpp  tests/$test_files.cpp	-o _test.exe	-D NDEBUG	-I inc	-I tests	-lstdc++


if [ -f "_test.exe" ]; then
    echo	==========================================
    echo		run tests now ...
    ./_test.exe	--version
    echo	==========================================
    ./_test.exe	--output=color
fi
