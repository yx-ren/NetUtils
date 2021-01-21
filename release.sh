#/bin/bash
CMAKE_FILE="CMakeLists.txt"

CURRENT_MAJOR_VERSION=$(grep "COMMON_MAJOR_VERSION" $CMAKE_FILE | head -1 | awk '{print $2}' | cut -d ')' -f 1)
CURRENT_MINOR_VERSION=$(grep "COMMON_MINOR_VERSION" $CMAKE_FILE | head -1 | awk '{print $2}' | cut -d ')' -f 1)
CURRENT_PATCH_VERSION=$(grep "COMMON_PATCH_VERSION" $CMAKE_FILE | head -1 | awk '{print $2}' | cut -d ')' -f 1)
VERSION_PREFIX=$CURRENT_MAJOR_VERSION.$CURRENT_MINOR_VERSION.$CURRENT_PATCH_VERSION
CURRENT_BUILD_NO=$(grep "COMMON_BUILD_NO" $CMAKE_FILE | head -1 | awk '{print $2}' | cut -d ')' -f 1)

GITLOG_HEAD_COMMIT=$(git log --decorate --oneline | head -n 1)
MACHED_TAG=$(echo $GITLOG_HEAD_COMMIT | grep -o $VERSION_PREFIX.$CURRENT_BUILD_NO )
echo $MACHED_TAG
if [ x"$MACHED_TAG" != x"" ] ; then
    echo "version $VERSION_PREFIX.$CURRENT_BUILD_NO was the newest, not update version"
    exit 0
fi

NEXT_BUILD_NO=$(echo $CURRENT_BUILD_NO | awk '{printf("%03d\n", $1 + 1);}')

CURRENT_BUILD_TAG="COMMON_BUILD_NO $CURRENT_BUILD_NO"
NEXT_BUILD_TAG="COMMON_BUILD_NO $NEXT_BUILD_NO"

sed -i "s/$CURRENT_BUILD_TAG/$NEXT_BUILD_TAG/" $CMAKE_FILE
GIT_COMMIT_COMMENT="modify project version from '$VERSION_PREFIX.$CURRENT_BUILD_NO' to '$VERSION_PREFIX.$NEXT_BUILD_NO'"
echo $GIT_COMMIT_COMMENT
git add -u
git commit -m "$GIT_COMMIT_COMMENT"
git tag $VERSION_PREFIX.$NEXT_BUILD_NO
