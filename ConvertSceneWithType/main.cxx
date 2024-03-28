/****************************************************************************************
    Based <FBX_SDK>/samples/ConvertScene/main.cxx
    Author: zhaoyafei0210@gmail.com
****************************************************************************************/

/////////////////////////////////////////////////////////////////////////
//
// This program converts any file in a format supported by the FBX SDK 
// into DAE, FBX, 3DS, OBJ and DXF files. 
//
// Steps:
// 1. Initialize SDK objects.
// 2. Load a file(fbx, obj,...) to a FBX scene.
// 3. Create a exporter.
// 4. Retrieve the writer ID according to the description of file format.
// 5. Initialize exporter with specified file format
// 6. Export.
// 7. Destroy the exporter
// 8. Destroy the FBX SDK manager
//
/////////////////////////////////////////////////////////////////////////

#include <fbxsdk.h>
#include <map>

#include "../Common/Common.h"

#define SAMPLE_FILENAME "box.fbx"

const char* lFileTypes[] =
{
    "_fbx7binary.fbx", "FBX binary (*.fbx)",
    "_fbx7ascii.fbx",  "FBX ascii (*.fbx)",
    "_fbx6binary.fbx", "FBX 6.0 binary (*.fbx)",
    "_fbx6ascii.fbx",  "FBX 6.0 ascii (*.fbx)",
    "_obj.obj",            "Alias OBJ (*.obj)",
    "_dae.dae",            "Collada DAE (*.dae)",
    "_dxf.dxf",            "AutoCAD DXF (*.dxf)"
};

std::map<std::string, int> lFileTypesMap {
    {"fbx", 0},
    {"fbx7", 0},
    {"fbx7b", 0},
    {"fbx7binary", 0},
    {"fbx7a", 1},
    {"fbx7ascii", 1},
    {"fbx6", 2},
    {"fbx6b", 2},
    {"fbx6binary", 2},
    {"fbx6a", 3},
    {"fbx6ascii", 3},
    {"obj", 4},
    {"dae", 5},
    {"dxf", 6}
};

int main(int argc, char** argv)
{
	FbxString lFilePath("");
    FbxString lOutputFilePath("");

    int i = 0;
    FbxString lFileType("fbx7");
    int lFileTypeIndex = lFileTypesMap[lFileType.Buffer()];

    // Parsing command line arguments.
    while(i < argc)
	{
        ++i;
		if( FbxString(argv[i]) == "-type" ) lFileType = argv[++i];
        else if( FbxString(argv[i]) == "-help" )
        {
            FBXSDK_printf("Usage: ConvertSceneWithType [options] <input file> [<output file>]\n");
            FBXSDK_printf("\t-type <fbx7|fbx6|fbx7a|fbx6a|obj|dxf|dae>, default fbx7a\n");

            std::map<std::string, int>::iterator it;
        
            for (it = lFileTypesMap.begin(); it!=lFileTypesMap.end(); it++)
            {
                int idx = it->second;
                // int idx = lFileTypesMap[it->first];
                FBXSDK_printf("\t\t%s: %s %s\n", it->first.c_str(), lFileTypes[idx*2], lFileTypes[idx*2+1]);
            }
            return 0;
        }
		else if( lFilePath.IsEmpty() ) lFilePath = argv[i];
        else if( lOutputFilePath.IsEmpty() ) lOutputFilePath = argv[i];
	}

	if( lFilePath.IsEmpty() ) lFilePath = SAMPLE_FILENAME;

    lFileTypeIndex = lFileTypesMap[lFileType.Buffer()];
    // lFileTypeIndex = lFileTypesMap["fbx6"];

    FBXSDK_printf("Input file:\t%s\n", lFilePath.Buffer());
    FBXSDK_printf("Output file prefix:\t%s\n", lOutputFilePath.Buffer());
    FBXSDK_printf("Type option:\t%s\n", lFileType.Buffer());
    FBXSDK_printf("lFileTypeIndex:\t%d\n", lFileTypeIndex);
    FBXSDK_printf("Output file suffix:\t%s\n", lFileTypes[lFileTypeIndex*2]);
    FBXSDK_printf("Output type desc:\t%s\n", lFileTypes[lFileTypeIndex*2+1]);

    FbxManager* lSdkManager = NULL;
    FbxScene* lScene = NULL;

    // Prepare the FBX SDK.
    InitializeSdkObjects(lSdkManager, lScene);

	bool lResult = LoadScene(lSdkManager, lScene, lFilePath.Buffer());
    if( lResult )
    {	
        int lFormat = lSdkManager->GetIOPluginRegistry()->FindWriterIDByDescription(lFileTypes[lFileTypeIndex*2+1]);
        size_t lFileNameLength  = 0;
        char* lNewFileName = NULL;

        // Construct the output file name.
        if( lOutputFilePath.IsEmpty() )
        {
            lFileNameLength = strlen(lFilePath.Buffer())-4;
            lNewFileName = new char[lFileNameLength+64];
            FBXSDK_strcpy(lNewFileName,lFileNameLength,lFilePath.Buffer());
        }
        else
        {
            lFileNameLength = strlen(lOutputFilePath.Buffer());
            lNewFileName = new char[lFileNameLength+64];
            FBXSDK_strcpy(lNewFileName,lFileNameLength+64,lOutputFilePath.Buffer());
        }
        FBXSDK_strcpy(lNewFileName+lFileNameLength,60, lFileTypes[lFileTypeIndex*2]);
        
        // Create an exporter.
        FbxExporter* lExporter = FbxExporter::Create(lSdkManager, "");

        // Initialize the exporter.
        lResult = lExporter->Initialize(lNewFileName, lFormat, lSdkManager->GetIOSettings());
        if( !lResult )
        {
            FBXSDK_printf("%s:\tCall to FbxExporter::Initialize() failed.\n", lFileTypes[i*2+1]);
            FBXSDK_printf("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
        }
        else
        {
            // Export the scene.
            lResult = lExporter->Export(lScene);
            if( !lResult )
            {
                FBXSDK_printf("Call to FbxExporter::Export() failed.\n");
            }
            else
            {
                FBXSDK_printf("%s:\tExport completed.\n", lFileTypes[i*2+1]);
                FBXSDK_printf("Export to %s.\n", lNewFileName);
            }
        }

        if (lNewFileName)
        {
            delete[] lNewFileName;
        }

        // Destroy the exporter.
        lExporter->Destroy();
    }
	else
    {
        FBXSDK_printf("Call to LoadScene() failed.\n");
    }

    // Delete the FBX SDK manager. All the objects that have been allocated 
    // using the FBX SDK manager and that haven't been explicitly destroyed 
    // are automatically destroyed at the same time.
    DestroySdkObjects(lSdkManager, lResult);
    return 0;
}

