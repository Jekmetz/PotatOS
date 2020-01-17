#!/usr/bin/sh

# Format every .c and .h
clang-format -i $(find ./ -name "*.c" -o -name "*.h")
