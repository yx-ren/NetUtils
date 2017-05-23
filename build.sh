#!/bin/bash

BUILD_TYPE=release
BUILD_DIR=build
UNAME=$(uname)

if [ ${UNAME} = "Darwin" ] ; then
    export MACOSX_DEPLOYMENT_TARGET=10.9
fi 

function get_cpu_count()
{
    if [ ${UNAME} = "Darwin" ] ; then
        CPU_COUNT=$(sysctl -n hw.ncpu)
    else
        CPU_COUNT=$(grep processor /proc/cpuinfo | wc -l)
    fi
}

function usage()
{
    echo "Usage: $0 [ debug | release ]" >&2
    echo "Default options: release" >&2
    exit 1
}

for arg in "$@"
do
    if [ $arg = "debug" -o $arg = "release" ]; then
        BUILD_TYPE=$arg
    else
        usage
    fi
done

echo "BUILD_TYPE:[$BUILD_TYPE]"

get_cpu_count
if [ x$CPU_COUNT = x ] ; then
   CPU_COUNT=4
fi
echo "cpu count:[${CPU_COUNT}]"

if [ ${UNAME} ] ; then
    echo "build OS:[${UNAME}]"
fi

echo "remove build directory:[${BUILD_DIR}]"
rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}

pushd ${BUILD_DIR}
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCPU_COUNT=$CPU_COUNT ..
make VERBOSE=1 -j ${CPU_COUNT}
popd
