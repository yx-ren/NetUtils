#/bin/bash
submod_common=./.submodule/common
pushd $submod_common
./build.sh

submod_common_build_dir=./build
pushd $submod_common_build_dir
make package

package=$(ls | grep -P ".tar.gz$")
if [ x"$package" == x"" ]; then
    echo "can't found the package, stop build"
    exit 1
fi

echo $package
package_version=$(echo $package | grep -P -o "\d+\.\d+\.\d+\-\d+" )
echo $package_version

extract_dir=../../internal
if [ ! -d $extract_dir ]; then
    echo "$extract_dir dir not exited, create it"
    mkdir -p $extract_dir
fi

tar -zxvf $package -C $extract_dir

popd # $submod_common_build_dir
popd # $submod_common
