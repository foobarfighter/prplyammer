#!/bin/bash

cd "$(dirname "$0")"

cd ../build
make && ctest --output-on-failure .
