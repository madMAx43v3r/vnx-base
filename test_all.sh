#!/bin/bash

set -e

cd build

./test/test_input_output > /dev/null

./test/test_variant > /dev/null

./test/test_config -c ../test/config/ > /dev/null

