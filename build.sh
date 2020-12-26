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

REDHAT_RELEASE=$(grep "Red Hat" /etc/redhat-release 2>/dev/null)
if [ x"$REDHAT_RELEASE" != x"" ] ; then
    LINUX_OS="RHEL"
else
    LINUX_OS="Debian"
fi
echo "Linux OS: $LINUX_OS"

echo "install the dependency thirdpart libraries ..."
if [ ${UNAME} = "Linux" ] ; then
    if [ ${LINUX_OS} = "RHEL" ]; then
        ./pre-build-yum.sh
    else
        ./pre-build.sh
    fi
fi

echo "build submodule"
./submod_build.sh
if [ $? != 0 ]; then
    echo "build submodule failed"
    exit 1
fi

echo "remove build directory:[${BUILD_DIR}]"
rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}

pushd ${BUILD_DIR}
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCPU_COUNT=$CPU_COUNT ..
make VERBOSE=1 -j ${CPU_COUNT}
popd
