#!/bin/bash

cd "$(dirname "$0")"

source loadenv.sh

export CK_FORK=no
cd ../build
make && gdb tests/check_main
