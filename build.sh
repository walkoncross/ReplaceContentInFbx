project=$1

export FBXSDK_ROOT="/Applications/Autodesk/FBX SDK/2020.3.1/"
export FBXSDK_LIB_FILE="$FBXSDK_ROOT/lib/clang/release/libfbxsdk.dylib"
# export FBXSDK_LIB_FILE="$FBXSDK_ROOT/lib/clang/release/libfbxsdk.a"

echo "FBXSDK_ROOT: $FBXSDK_ROOT"
echo "FBXSDK_LIB_FILE: $FBXSDK_LIB_FILE"

mkdir -p $project/build && pushd $project/build
# cmake -DFBX_ARCH=arm64 -DFBX_SHARED=1 -DFBX_VARIANT=release ..
cmake -DFBX_SHARED=1 -DFBX_VARIANT=release ..

make -j 4

popd

ln -s "$FBXSDK_LIB_FILE" ./bin || echo "Symlink already exists. Skipping linking......"
