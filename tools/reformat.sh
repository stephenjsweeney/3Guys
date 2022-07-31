#!/bin/bash

find ../src -iname *.c -exec python tidyHeaders.py {} \;

cd ..

find -regex '.*\.\(c\|h\)' -exec clang-format -style=file -i {} \;
