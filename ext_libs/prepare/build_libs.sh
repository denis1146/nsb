#!/usr/bin/env bash

basePath=$(pwd)
targetLibs="${basePath}/.."
tmpPath="${basePath}/tmp"
isLinux=true
if $isLinux;
then
  threads=$(lscpu | grep -i "cpu(s):" | head -n 1 | tr " " "\n" | tail -n 1)
else
  threads=16
fi
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
  if $isLinux;
  then
    cmake --build . -- -j $threads
  else
    cmake --build . --config Debug -- -maxCpuCount
    cmake --build . --config Release -- -maxCpuCount
  fi
fi

# Installatio
cd "${boostPath}/_build"
if $isLinux;
then
  cmake --install . --prefix "${targetLibs}/${boost_1_88_0}"
else
  cmake --install . --config Debug --prefix "${targetLibs}/${boost_1_88_0}/Debug"
  cmake --install . --config Release --prefix "${targetLibs}/${boost_1_88_0}/Release"
fi
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
  if $isLinux;
  then
    cmake --build . -- -j $threads
  else
    cmake --build . --config Debug -- -maxCpuCount
    cmake --build . --config Release -- -maxCpuCount
  fi
fi

# Installation
cd "${gtestPath}/_build"
if $isLinux;
then
  cmake --install . --prefix "${targetLibs}/${gtest_1_17_0}"
else
  cmake --install . --config Debug --prefix "${targetLibs}/${gtest_1_17_0}/Debug"
  cmake --install . --config Release --prefix "${targetLibs}/${gtest_1_17_0}/Release"
fi
# ~GTest
