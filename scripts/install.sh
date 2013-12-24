#!/bin/bash

cd "$(dirname "$0")"

cd ../build
make && sudo make install
