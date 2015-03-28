# ! /bin/bash

source  tools/run_test_cases.sh	_test_*

if [ -f "_test.exe" ]; then
rm  _test.exe
fi

if [ -f "*.module" ]; then
rm  *.module
fi
