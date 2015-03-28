#!/bin/bash

astyle_proc=astyle
if [ `uname -s` == 'Darwin' ]; then
    astyle_proc=tools/astyle.Darwin
fi

for src_dir in inc/simple src/simple tests ; do
find $src_dir -maxdepth 1 \( -name '*.h' -o -name '*.c' -o -name '*.cpp' -o -name '*.inc' \) -exec $astyle_proc --style=linux --suffix=none --formatted '{}' > /dev/null \;
find $src_dir -maxdepth 1 \( -name '*.h' -o -name '*.c' -o -name '*.cpp' -o -name '*.inc' \) -exec $astyle_proc --style=java --suffix=none --formatted '{}' > /dev/null \;
done
