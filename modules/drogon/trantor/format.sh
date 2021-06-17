#!/usr/bin/env bash

clang-format --version
find net utils -name *.h -o -name *.cc -exec dos2unix {} \;
find net utils -name *.h -o -name *.cc|xargs clang-format -i -style=file
