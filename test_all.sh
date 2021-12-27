#!/bin/bash

set -e

./build/test/test_input_output > /dev/null

./build/test/test_variant > /dev/null

./build/test/test_config -c test/config/ > /dev/null

