#!/usr/bin/env bash

windows="windows"
linux="linux"

if [ $# -ne 1 ] || [ $1 != $linux ] && [ $1 != $windows ];
then
  echo "Error: Please pass the operating system name. Use <$linux> or <$windows> as a parameter." >&2
  echo "usage: $0 $linux" >&2
  echo "or" >&2
  echo "usage: $0 $windows" >&2

  exit 1
fi

basePath=$(pwd)
targetLibs="${basePath}/.."
tmpPath="${basePath}/tmp"

if [ $1 == $linux ];
then
  threads=$(nproc 2>/dev/null \
            || echo 16 \
  )
  echo "CPU(s): ${threads}"
  buildOptions=" -j ${threads} "
else
  buildOptions=" -maxCpuCount "
fi

#####     Functions     ######
# gitClone(repo, branch, targetPath, force=0)
function gitClone {
  echo "gitClone( $@ ) [$#]"
  if [ $# -ne 3 ] && [ $# -ne 4 ]
  then
    echo "Error: Incorrect number of arguments passed to function ${0}" >&2
    return 1
  fi

  local repo=$1
  local branch=$2
  local targetPath="${3}"
  local force=$4
  local currentPath=$(pwd)

  if [ $force -eq "1" ]
  then 
    rm -rf "${targetPath}"
  fi

  if test -d "${targetPath}";
  then 
    echo "Found a catalog with ${targetPath}"
    return 0
  fi

  mkdir -p "${targetPath}"
  git clone "${repo}" -b "${branch}" --depth 1 "${targetPath}"
  cd "${targetPath}"
  git submodule update --depth 1 --init --recursive
  cd "${currentPath}"
  return 0
}

# buildProject(projectPath, buildCatalog, force=0)
function buildProject {
  echo "buildProject( $@ ) [$#]"
  if [ $# -ne 2 ] && [ $# -ne 3 ]
  then
    echo "Error: Incorrect number of arguments passed to function ${0}" >&2
    return 1
  fi

  local projectPath="${1}"
  local buildCatalog="${2}"
  local force=$3
  local buildPath="${projectPath}/${buildCatalog}"
  local currentPath=$(pwd)

  if [ $force -eq "1" ]
  then 
    rm -rf "${buildPath}"
  fi

  if test -d "${buildPath}";
  then 
    echo "Found a catalog with ${buildPath}"
    return 0
  fi

  mkdir -p "${buildPath}"
  cd "${buildPath}"

  cmake -S ../ -B ./debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_DEBUG_POSTFIX=d
  cmake -S ../ -B ./release -DCMAKE_BUILD_TYPE=Release
  cmake --build ./debug --config Debug -- $buildOptions
  cmake --build ./release --config Release -- $buildOptions
  cd "${currentPath}"
  return 0
}

# installProject(projectBuildPath, installPath, force=0)
function installProject {
  echo "installProject( $@ ) [$#]"
  if [ $# -ne 2 ] && [ $# -ne 3 ]
  then
    echo "Error: Incorrect number of arguments passed to function ${0}" >&2
    return 1
  fi

  local projectBuildPath="${1}"
  local installPath="${2}"
  local force=$3

  if [ $force -eq "1" ]
  then 
    rm -rf "${installPath}"
  fi

  if test -d "${installPath}";
  then 
    echo "Found a catalog with ${installPath}"
    return 0
  fi

  echo "${installPath}"
  cmake --install "${projectBuildPath}/debug" --config Debug --prefix "${installPath}"
  cmake --install "${projectBuildPath}/release" --config Release --prefix "${installPath}"
  return 0
}
#####     ~Functions     #####

# GTest
gtest_1_17_0="gtest_1_17_0"
gtestPath="${tmpPath}/${gtest_1_17_0}"
gitClone "https://github.com/google/googletest.git" "v1.17.0" "${gtestPath}"
buildProject "${gtestPath}" "_build"
installProject "${gtestPath}/_build" "${targetLibs}/${gtest_1_17_0}"

# Boost
boost_1_88_0="boost_1_88_0"
boostPath="${tmpPath}/${boost_1_88_0}"
gitClone "https://github.com/boostorg/boost.git" "boost-1.88.0" "${boostPath}"
buildProject "${boostPath}" "_build"
installProject "${boostPath}/_build" "${targetLibs}/${boost_1_88_0}"
