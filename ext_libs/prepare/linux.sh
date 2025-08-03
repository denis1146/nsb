#!/usr/bin/env bash

basePath=$(pwd)
targetLibs="${basePath}/.."
tmpPath="${basePath}/tmp"

threads=$(lscpu | grep -i "cpu(s):" | head -n 1 | tr " " "\n" | tail -n 1)
echo "CPU(s): ${threads}"

# Boost
boost_1_88_0="boost_1_88_0"
boostPath="${tmpPath}/${boost_1_88_0}"

# Download
if test -d "${boostPath}";
then 
  echo "Found a catalog with ${boost_1_88_0} (${boostPath})"
else
  rm -rf "${boostPath}"
  mkdir -p "${boostPath}"
  git clone https://github.com/boostorg/boost.git -b boost-1.88.0 --depth 1 "${boostPath}"
  cd "${boostPath}"
  git submodule update --depth 1 --init --recursive
fi

# Build
cd "${boostPath}"
if test -d "${boostPath}/_build";
then 
  echo "Found catalog with ${boost_1_88_0} build location (${boostPath}/_build)"
else
  mkdir _build
  cd _build
  cmake ..
  cmake --build . -- -j $threads
fi

# Installatio
cd "${boostPath}/_build"
cmake --install . --prefix "${targetLibs}/${boost_1_88_0}"
# ~Boost

# GTest
gtest_1_17_0="gtest_1_17_0"
gtestPath="${tmpPath}/${gtest_1_17_0}"

# Download
if test -d "${gtestPath}";
then 
  echo "Found a catalog with ${gtest_1_17_0} (${gtestPath})"
else
  rm -rf "${gtestPath}"
  mkdir -p "${gtestPath}"
  git clone https://github.com/google/googletest.git -b v1.17.0 --depth 1 "${gtestPath}"
  cd "${gtestPath}"
  git submodule update --depth 1 --init --recursive
fi

# Build
cd "${gtestPath}"
if test -d "${gtestPath}/_build";
then 
  echo "Found catalog with ${gtest_1_17_0} build location (${gtestPath}/_build)"
else
  mkdir _build
  cd _build
  cmake ..
  cmake --build . -- -j $threads
fi

# Installation
cd "${gtestPath}/_build"
cmake --install . --prefix "${targetLibs}/${gtest_1_17_0}"

# ~GTest
