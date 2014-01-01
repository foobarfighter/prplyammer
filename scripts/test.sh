#!/bin/bash

cd "$(dirname "$0")"

source loadenv.sh

cd ../build
make && ctest --output-on-failure .
