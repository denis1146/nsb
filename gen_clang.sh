#!/usr/bin/env bash

export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
cmake -S ./ -B ./build_clang
