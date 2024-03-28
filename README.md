# fbx-tools

author: zhaoyafei0210@gmail.com

## Prerequisites

Download and install FBX SDK from [Autodesk FBX SDK](https://aps.autodesk.com/developer/overview/fbx-sdk)

## ConvertSceneWithType
Convert FBX scene to another type, using FBX SDK

### build
1. set environment variables in ./build.sh;
2. run:
```bash
./build.sh ConvertSceneWithType
```
### usage
```bash
./bin/ConvertSceneWithType -help
```
which prints:
```
Usage: ConvertSceneWithType [options] <input file> [<output file>]
	-type <fbx7|fbx6|fbx7a|fbx6a|obj|dxf|dae>, default fbx7a
		dae: _dae.dae Collada DAE (*.dae)
		dxf: _dxf.dxf AutoCAD DXF (*.dxf)
		fbx: _fbx7binary.fbx FBX binary (*.fbx)
		fbx6: _fbx6binary.fbx FBX 6.0 binary (*.fbx)
		fbx6a: _fbx6ascii.fbx FBX 6.0 ascii (*.fbx)
		fbx6ascii: _fbx6ascii.fbx FBX 6.0 ascii (*.fbx)
		fbx6b: _fbx6binary.fbx FBX 6.0 binary (*.fbx)
		fbx6binary: _fbx6binary.fbx FBX 6.0 binary (*.fbx)
		fbx7: _fbx7binary.fbx FBX binary (*.fbx)
		fbx7a: _fbx7ascii.fbx FBX ascii (*.fbx)
		fbx7ascii: _fbx7ascii.fbx FBX ascii (*.fbx)
		fbx7b: _fbx7binary.fbx FBX binary (*.fbx)
		fbx7binary: _fbx7binary.fbx FBX binary (*.fbx)
		obj: _obj.obj Alias OBJ (*.obj)
```

```bash
./bin/ConvertSceneWithType -type fbx7a ./data/box.fbx ./data/box
```
which prints:
```
Input file:	./data/box.fbx
Output file prefix:	./data/box
Type option:	fbx7a
lFileTypeIndex:	1
Output file suffix:	_fbx7ascii.fbx
Output type desc:	FBX ascii (*.fbx)
Autodesk FBX SDK version 2020.3.1 Release (325fc9c4c)
FBX file format version for this FBX SDK is 7.7.0
FBX file format version for file './data/box.fbx' is 7.1.0

Animation Stack Information
    Number of Animation Stacks: 1
    Current Animation Stack: "Take 001"

    Animation Stack 0
         Name: "Take 001"
         Description: ""
         Import Name: "Take 001"
         Import State: true

Collada DAE (*.dae):	Export completed.
Export to ./data/box_fbx7ascii.fbx.
Program Success!
```

## ReplaceContentInFbx
Replace FBX contents (mesh, blendshape, etc) with new one, using FBX SDK

### build

### usage