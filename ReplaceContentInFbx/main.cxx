/****************************************************************************************
    Based on <FBX_SDK>/samples/ImportScene/main.cxx
    Author: zhaoyafei0210@gmail.com
****************************************************************************************/

/////////////////////////////////////////////////////////////////////////
//
// This example illustrates how to detect if a scene is password 
// protected, import and browse the scene to access node and animation 
// information. It displays the content of the FBX file which name is 
// passed as program argument. You can try it with the various FBX files 
// output by the export examples.
//
/////////////////////////////////////////////////////////////////////////

#include "../Common/Common.h"
#include "DisplayCommon.h"
#include "DisplayHierarchy.h"
#include "DisplayAnimation.h"
#include "DisplayMarker.h"
#include "DisplaySkeleton.h"
#include "DisplayMesh.h"
#include "DisplayNurb.h"
#include "DisplayPatch.h"
#include "DisplayLodGroup.h"
#include "DisplayCamera.h"
#include "DisplayLight.h"
#include "DisplayGlobalSettings.h"
#include "DisplayPose.h"
#include "DisplayPivotsAndLimits.h"
#include "DisplayUserProperties.h"
#include "DisplayGenericInfo.h"

#include <queue>
#include "ReplaceMesh.h"

#define DEFAULT_OUTPUT_FILENAME "modifyscene-output.fbx"

// Local function prototypes.
void DisplayContent(FbxScene* pScene);
void DisplayContent(FbxNode* pNode);
void DisplayTarget(FbxNode* pNode);
void DisplayTransformPropagation(FbxNode* pNode);
void DisplayGeometricTransform(FbxNode* pNode);
void DisplayMetaData(FbxScene* pScene);

bool ReplaceContent(FbxScene*& pScene, FbxString& pNodeName, FbxScene*& pScene2, FbxString& pNodeName2);

static bool gVerbose = true;

int main(int argc, char** argv)
{
    FbxManager* lSdkManager = NULL;
    FbxScene* lScene = NULL;
    FbxManager* lSdkManager2 = NULL;
    FbxScene* lScene2 = NULL;
    bool lResult;
    bool lResult2;

    // The example can take a FBX file as an argument.
	FbxString lFilePath("");
    FbxString lFilePath2("");
	FbxString lNodeName("");
	FbxString lNodeName2("");
    // The example can take an output file name as an argument.
	const char* lSampleFileName = NULL;

	for( int i = 1, c = argc; i < c; ++i )
	{
		if( FbxString(argv[i]) == "-test" ) 
        {
            gVerbose = false;
        }
		else if( lFilePath.IsEmpty() ) 
        {
            lFilePath = argv[i];
        }
        else if ( lNodeName.IsEmpty() ) 
        {
            lNodeName = argv[i];
        } 
        else if( lFilePath2.IsEmpty() ) 
        {
            lFilePath2 = argv[i];
        }
        else if ( lNodeName2.IsEmpty() ) 
        {
            lNodeName2 = argv[i];
        }
        else if( !lSampleFileName ) 
        {
            lSampleFileName = argv[i];
        }
	}

    if( !lSampleFileName ) lSampleFileName = DEFAULT_OUTPUT_FILENAME;

	if( lFilePath.IsEmpty() || lNodeName.IsEmpty() || lFilePath2.IsEmpty() || lNodeName2.IsEmpty() )
	{
        FBXSDK_printf("\n\nUsage: ModifyScene <FBX file1 name> <node1 name> <FBX file2 name> <node2 name> [output FBX]\n\n");
        FBXSDK_printf("\n\nExit...");
        exit(-1);
    }

    FBXSDK_printf("\n\nLoad File: %s\n\n", lFilePath.Buffer());

    // Prepare the FBX SDK.
    InitializeSdkObjects(lSdkManager, lScene);
    // Load the scene.

    lResult = LoadScene(lSdkManager, lScene, lFilePath.Buffer());

    if(lResult == false)
    {
        FBXSDK_printf("\n\nAn error occurred while loading the scene...");

        // Destroy all objects created by the FBX SDK.
        DestroySdkObjects(lSdkManager, lResult);
        FBXSDK_printf("\n\nExit...");
        exit(-1);
    }
    else 
    {
        // Display the scene.
        DisplayMetaData(lScene);

        FBXSDK_printf("\n\n---------------------\nGlobal Light Settings\n---------------------\n\n");

        if( gVerbose ) DisplayGlobalLightSettings(&lScene->GetGlobalSettings());

        FBXSDK_printf("\n\n----------------------\nGlobal Camera Settings\n----------------------\n\n");

        if( gVerbose ) DisplayGlobalCameraSettings(&lScene->GetGlobalSettings());

        FBXSDK_printf("\n\n--------------------\nGlobal Time Settings\n--------------------\n\n");

        // if( gVerbose ) DisplayGlobalTimeSettings(&lScene->GetGlobalSettings());

        // FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");

        if( gVerbose ) DisplayHierarchy(lScene);

        // FBXSDK_printf("\n\n------------\nNode Content\n------------\n\n");

        // if( gVerbose ) DisplayContent(lScene);

        // FBXSDK_printf("\n\n----\nPose\n----\n\n");

        // if( gVerbose ) DisplayPose(lScene);

        // FBXSDK_printf("\n\n---------\nAnimation\n---------\n\n");

        // if( gVerbose ) DisplayAnimation(lScene);

        // //now display generic information

        // FBXSDK_printf("\n\n---------\nGeneric Information\n---------\n\n");
        // if( gVerbose ) DisplayGenericInfo(lScene);
    }

    InitializeSdkObjects(lSdkManager2, lScene2);
    // Load the scene.

    FBXSDK_printf("\n\nLoad File: %s\n\n", lFilePath2.Buffer());

    // Prepare the FBX SDK.
    InitializeSdkObjects(lSdkManager2, lScene2);
    // Load the scene.

    lResult2 = LoadScene(lSdkManager2, lScene2, lFilePath2.Buffer());

    if(lResult2 == false)
    {
        FBXSDK_printf("\n\nAn error occurred while loading the scene...");

        DestroySdkObjects(lSdkManager, lResult);
        DestroySdkObjects(lSdkManager2, lResult2);

        FBXSDK_printf("\n\nExit...");
        exit(-1);
    }
    else 
    {
        // Display the scene.
        DisplayMetaData(lScene);

        FBXSDK_printf("\n\n---------------------\nGlobal Light Settings\n---------------------\n\n");

        if( gVerbose ) DisplayGlobalLightSettings(&lScene->GetGlobalSettings());

        FBXSDK_printf("\n\n----------------------\nGlobal Camera Settings\n----------------------\n\n");

        if( gVerbose ) DisplayGlobalCameraSettings(&lScene->GetGlobalSettings());

        FBXSDK_printf("\n\n--------------------\nGlobal Time Settings\n--------------------\n\n");

        if( gVerbose ) DisplayGlobalTimeSettings(&lScene->GetGlobalSettings());

        // FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");

        // if( gVerbose ) DisplayHierarchy(lScene);

        // FBXSDK_printf("\n\n------------\nNode Content\n------------\n\n");

        // if( gVerbose ) DisplayContent(lScene);

        // FBXSDK_printf("\n\n----\nPose\n----\n\n");

        // if( gVerbose ) DisplayPose(lScene);

        // FBXSDK_printf("\n\n---------\nAnimation\n---------\n\n");

        // if( gVerbose ) DisplayAnimation(lScene);

        // //now display generic information

        // FBXSDK_printf("\n\n---------\nGeneric Information\n---------\n\n");
        // if( gVerbose ) DisplayGenericInfo(lScene);
    }

    // Replace the node in the first scene by the node in the second scene.
    lResult = ReplaceContent(lScene, lNodeName, lScene2, lNodeName2);

    if(lResult == false)
    {
        DestroySdkObjects(lSdkManager, lResult);
        DestroySdkObjects(lSdkManager2, lResult2);

        FBXSDK_printf("\n\nFalied to ReplaceContent(), Exit...");
        exit(-1);
    }

    // Save the scene.

	lResult = SaveScene(lSdkManager, lScene, lSampleFileName, 0, true);
    if(lResult == false)
    {
        FBXSDK_printf("\n\nAn error occurred while saving the scene...\n");
        DestroySdkObjects(lSdkManager, lResult);
        return 0;
    }

    // Destroy all objects created by the FBX SDK.
    DestroySdkObjects(lSdkManager, lResult);
    DestroySdkObjects(lSdkManager2, lResult2);

    return 0;
}

void DisplayContent(FbxScene* pScene)
{
    int i;
    FbxNode* lNode = pScene->GetRootNode();

    if(lNode)
    {
        for(i = 0; i < lNode->GetChildCount(); i++)
        {
            DisplayContent(lNode->GetChild(i));
        }
    }
}

FbxNode* GetNodeByName_BFS(FbxNode* pRootNode, FbxString& pNodeName)
{
    int i;
    std::queue<FbxNode*> lQueue;
    FbxNode* lMatchedNode = NULL;

    if (pRootNode)
    {
        lQueue.push(pRootNode);
    }

    while (lQueue.empty() == false)
    {
        FbxNode* pNode = lQueue.front();
        lQueue.pop();

        if (pNode->GetName() == pNodeName.Buffer() )
        {
            FBXSDK_printf("Found node: %s\n", pNode->GetName());
            lMatchedNode = pNode;
            break;
        }
        else
        {
            for(i = 0; i < pNode->GetChildCount(); i++)
            {
                lQueue.push(pNode->GetChild(i));
            }
        }
    }
    
    return lMatchedNode;
}   

bool ReplaceContent(FbxScene*& pScene, FbxString& pNodeName, FbxScene*& pScene2, FbxString& pNodeName2)
{
    bool lResult = false;
    int i;
    FbxNode* lNode = pScene->GetRootNode();
    FbxNode* lNode2 = pScene2->GetRootNode();

    FbxNode* lMatchedNode = NULL;
    FbxNode* lMatchedNode2 = NULL;

    FbxNodeAttribute::EType lAttributeType;
    FbxNodeAttribute::EType lAttributeType2;


    if(lNode)
    {
        // lMatchedNode = GetNodeByName_BFS(lNode, pNodeName);
        lMatchedNode = lNode->FindChild(pNodeName.Buffer(), true);
    }
    else
    {
        FBXSDK_printf("Error: NULL root node for the first scene\n");
        return false;
    }

    if (lMatchedNode == NULL)
    {
        FBXSDK_printf("Error: Failed to find node with name %s in the first scene\n", pNodeName.Buffer());
        return false;
    }
    
    if(lNode2)
    {
        // lMatchedNode2 = GetNodeByName_BFS(lNode2, pNodeName2);
        lMatchedNode2 = lNode2->FindChild(pNodeName2.Buffer(), true);
    }
    else
    {
        FBXSDK_printf("Error: NULL root node for the second scene\n");
        return false;
    }


    if (lMatchedNode2 == NULL)
    {
        FBXSDK_printf("Error: Failed to find node with name %s in the second scene\n", pNodeName2.Buffer());
        return false;
    }

    if(lMatchedNode && lMatchedNode2)
    {
        FBXSDK_printf("Replacing node: %s in scene %s by %s in scene %s \n", lMatchedNode->GetName(), pScene->GetName(), lMatchedNode2->GetName(), pScene2->GetName());

        if(lMatchedNode->GetNodeAttribute() == NULL)
        {
            FBXSDK_printf("NULL Node Attribute for node %s\n\n", lMatchedNode->GetName());
        }

        if(lMatchedNode2->GetNodeAttribute() == NULL)
        {
            FBXSDK_printf("NULL Node Attribute for node %s\n\n", lMatchedNode2->GetName());
        }

        lAttributeType = (lMatchedNode->GetNodeAttribute()->GetAttributeType());
        lAttributeType2 = (lMatchedNode2->GetNodeAttribute()->GetAttributeType());

        if (lAttributeType != lAttributeType2)
        {
            FBXSDK_printf("Error: Nodes are not of the same type\n");
            return false;
        }
        else
        {
            switch (lAttributeType)
            {
            default:
                break;
            // case FbxNodeAttribute::eMarker:  
            //     lResult = ReplaceMarker(lMatchedNode, lMatchedNode);
            //     break;

            // case FbxNodeAttribute::eSkeleton:  
            //     lResult = ReplaceSkeleton(lMatchedNode, lMatchedNode);
            //     break;

            case FbxNodeAttribute::eMesh:      
                lResult = ReplaceMesh(lMatchedNode, lMatchedNode2);
                break;

            // case FbxNodeAttribute::eNurbs:      
            //     lResult = ReplaceNurb(lMatchedNode, lMatchedNode);
            //     break;

            // case FbxNodeAttribute::ePatch:     
            //     lResult = ReplacePatch(lMatchedNode, lMatchedNode);
            //     break;

            // case FbxNodeAttribute::eCamera:    
            //     lResult = ReplaceCamera(lMatchedNode, lMatchedNode);
            //     break;

            // case FbxNodeAttribute::eLight:     
            //     lResult = ReplaceLight(lMatchedNode, lMatchedNode);
            //     break;

            // case FbxNodeAttribute::eLODGroup:
            //     lResult = ReplaceLodGroup(lMatchedNode, lMatchedNode);
            //     break;
            }   
        }
    }

    return lResult;
}

void DisplayContent(FbxNode* pNode)
{
    FbxNodeAttribute::EType lAttributeType;
    int i;

    if(pNode->GetNodeAttribute() == NULL)
    {
        FBXSDK_printf("NULL Node Attribute\n\n");
    }
    else
    {
        lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

        switch (lAttributeType)
        {
	    default:
	        break;
        case FbxNodeAttribute::eMarker:  
            DisplayMarker(pNode);
            break;

        case FbxNodeAttribute::eSkeleton:  
            DisplaySkeleton(pNode);
            break;

        case FbxNodeAttribute::eMesh:      
            DisplayMesh(pNode);
            break;

        case FbxNodeAttribute::eNurbs:      
            DisplayNurb(pNode);
            break;

        case FbxNodeAttribute::ePatch:     
            DisplayPatch(pNode);
            break;

        case FbxNodeAttribute::eCamera:    
            DisplayCamera(pNode);
            break;

        case FbxNodeAttribute::eLight:     
            DisplayLight(pNode);
            break;

        case FbxNodeAttribute::eLODGroup:
            DisplayLodGroup(pNode);
            break;
        }   
    }

    DisplayUserProperties(pNode);
    DisplayTarget(pNode);
    DisplayPivotsAndLimits(pNode);
    DisplayTransformPropagation(pNode);
    DisplayGeometricTransform(pNode);

    for(i = 0; i < pNode->GetChildCount(); i++)
    {
        DisplayContent(pNode->GetChild(i));
    }
}


void DisplayTarget(FbxNode* pNode)
{
    if(pNode->GetTarget() != NULL)
    {
        DisplayString("    Target Name: ", (char *) pNode->GetTarget()->GetName());
    }
}

void DisplayTransformPropagation(FbxNode* pNode)
{
    FBXSDK_printf("    Transformation Propagation\n");

    // 
    // Rotation Space
    //
    EFbxRotationOrder lRotationOrder;
    pNode->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);

    FBXSDK_printf("        Rotation Space: ");

    switch (lRotationOrder)
    {
    case eEulerXYZ: 
        FBXSDK_printf("Euler XYZ\n");
        break;
    case eEulerXZY:
        FBXSDK_printf("Euler XZY\n");
        break;
    case eEulerYZX:
        FBXSDK_printf("Euler YZX\n");
        break;
    case eEulerYXZ:
        FBXSDK_printf("Euler YXZ\n");
        break;
    case eEulerZXY:
        FBXSDK_printf("Euler ZXY\n");
        break;
    case eEulerZYX:
        FBXSDK_printf("Euler ZYX\n");
        break;
    case eSphericXYZ:
        FBXSDK_printf("Spheric XYZ\n");
        break;
    }

    //
    // Use the Rotation space only for the limits
    // (keep using eEulerXYZ for the rest)
    //
    FBXSDK_printf("        Use the Rotation Space for Limit specification only: %s\n",
        pNode->GetUseRotationSpaceForLimitOnly(FbxNode::eSourcePivot) ? "Yes" : "No");


    //
    // Inherit Type
    //
    FbxTransform::EInheritType lInheritType;
    pNode->GetTransformationInheritType(lInheritType);

    FBXSDK_printf("        Transformation Inheritance: ");

    switch (lInheritType)
    {
    case FbxTransform::eInheritRrSs:
        FBXSDK_printf("RrSs\n");
        break;
    case FbxTransform::eInheritRSrs:
        FBXSDK_printf("RSrs\n");
        break;
    case FbxTransform::eInheritRrs:
        FBXSDK_printf("Rrs\n");
        break;
    }
}

void DisplayGeometricTransform(FbxNode* pNode)
{
    FbxVector4 lTmpVector;

    FBXSDK_printf("    Geometric Transformations\n");

    //
    // Translation
    //
    lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    FBXSDK_printf("        Translation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Rotation
    //
    lTmpVector = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
    FBXSDK_printf("        Rotation:    %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Scaling
    //
    lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
    FBXSDK_printf("        Scaling:     %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);
}


void DisplayMetaData(FbxScene* pScene)
{
    FbxDocumentInfo* sceneInfo = pScene->GetSceneInfo();
    if (sceneInfo)
    {
        FBXSDK_printf("\n\n--------------------\nMeta-Data\n--------------------\n\n");
        FBXSDK_printf("    Title: %s\n", sceneInfo->mTitle.Buffer());
        FBXSDK_printf("    Subject: %s\n", sceneInfo->mSubject.Buffer());
        FBXSDK_printf("    Author: %s\n", sceneInfo->mAuthor.Buffer());
        FBXSDK_printf("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
        FBXSDK_printf("    Revision: %s\n", sceneInfo->mRevision.Buffer());
        FBXSDK_printf("    Comment: %s\n", sceneInfo->mComment.Buffer());

        FbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
        if (thumbnail)
        {
            FBXSDK_printf("    Thumbnail:\n");

            switch (thumbnail->GetDataFormat())
            {
            case FbxThumbnail::eRGB_24:
                FBXSDK_printf("        Format: RGB\n");
                break;
            case FbxThumbnail::eRGBA_32:
                FBXSDK_printf("        Format: RGBA\n");
                break;
            }

            switch (thumbnail->GetSize())
            {
	        default:
	            break;
            case FbxThumbnail::eNotSet:
                FBXSDK_printf("        Size: no dimensions specified (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case FbxThumbnail::e64x64:
                FBXSDK_printf("        Size: 64 x 64 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case FbxThumbnail::e128x128:
                FBXSDK_printf("        Size: 128 x 128 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
            }
        }
    }
}

