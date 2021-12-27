#!/bin/bash

cd $(dirname "$0")

vnxcppcodegen $@ --cleanup generated/ vnx interface/ modules/ || exit 1
vnxcppcodegen --cleanup test/generated/ vnx.test interface/ modules/ test/interface/ || exit 1

