/****************************************************************************************
// Based on <FBX_SDK>/samples/ImportScene/DisplayMesh.cxx
// author: zhaoyafei0210@gmail.com
****************************************************************************************/
#include <fbxsdk.h>
#include "DisplayCommon.h"
#include "ReplaceMesh.h"

// #include "ReplaceMaterial.h"
// #include "ReplaceTexture.h"
// #include "ReplaceLink.h"
#include "ReplaceShape.h"
// #include "ReplaceCache.h"

#if defined (FBXSDK_ENV_MAC)
// disable the �format not a string literal and no format arguments� warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

#define MAT_HEADER_LENGTH 200

bool ReplaceControlsPoints(FbxMesh* pMesh, FbxMesh* pNewMesh);
// bool ReplacePolygons(FbxMesh* pMesh, FbxMesh* pNewMesh);
// bool ReplaceMaterialMapping(FbxMesh* pMesh, FbxMesh* pNewMesh);
// bool ReplaceTextureMapping(FbxMesh* pMesh, FbxMesh* pNewMesh);
// bool ReplaceTextureNames( FbxProperty &pProperty, FbxString& pConnectionString );
// bool ReplaceMaterialConnections(FbxMesh* pMesh, FbxMesh* pNewMesh);
// bool ReplaceMaterialTextureConnections( FbxSurfaceMaterial* pMaterial, 
//                                        char * header, int pMatId, int l );

bool ReplaceMesh(FbxNode* pNode, FbxNode* pNewNode)
{
	bool lResult = false;

    FbxMesh* lMesh = (FbxMesh*) pNode->GetNodeAttribute ();
    FbxMesh* lNewMesh = (FbxMesh*) pNewNode->GetNodeAttribute ();

    DisplayString("Old Mesh Name: ", (char *) pNode->GetName());
    DisplayString("New Mesh Name: ", (char *) pNewNode->GetName());

    // lResult = ReplaceMetaDataConnections(lMesh, lNewMesh);
    lResult = ReplaceControlsPoints(lMesh, lNewMesh);
    // lResult = ReplacePolygons(lMesh, lNewMesh);
    // lResult = ReplaceMaterialMapping(lMesh, lNewMesh);
    // lResult = ReplaceMaterial(lMesh, lNewMesh);
    // lResult = ReplaceTexture(lMesh, lNewMesh);
    // lResult = ReplaceMaterialConnections(lMesh, lNewMesh);
    // lResult = ReplaceLink(lMesh, lNewMesh);
    lResult = ReplaceShape(lMesh, lNewMesh);
	
	// lResult = ReplaceCache(lMesh, lNewMesh);

	return lResult;
}


bool ReplaceControlsPoints(FbxMesh* pMesh, FbxMesh* pNewMesh)
{
	bool lResult = false;
    int i, lControlPointsCount = pMesh->GetControlPointsCount();
    int lControlPointsCount2 = pNewMesh->GetControlPointsCount();

    FbxVector4* lControlPoints = pMesh->GetControlPoints();
    FbxVector4* lControlPoints2 = pNewMesh->GetControlPoints();

    // DisplayString("    Control Points");
	DisplayInt("    Control Points: ", lControlPointsCount);
	DisplayInt("    Control Points: ", lControlPointsCount2);

	if (lControlPointsCount != lControlPointsCount2)
	{
		DisplayString("    Warning: Control Points count mismatch");

		return lResult;
	}

    for (i = 0; i < lControlPointsCount; i++)
    {
		if (i < 100)
		{
			DisplayInt("        Control Point ", i);
			Display3DVector("            Coordinates A: ", lControlPoints[i]);
			Display3DVector("            Coordinates B: ", lControlPoints2[i]);
		}

		// for (int ii = 0; ii < 3; ii++)
		// {
		// 	lControlPoints[i][ii] = lControlPoints2[i][ii];
		// }
		
		lControlPoints[i].Set(lControlPoints2[i][0], lControlPoints2[i][1], lControlPoints2[i][2], lControlPoints2[i][3]);
		if (i < 100)
		{
			Display3DVector("            Coordinates A: ", lControlPoints2[i]);
		}
        for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
        {
            FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal( j);
            FbxGeometryElementNormal* leNormals2 = pNewMesh->GetElementNormal( j);

			if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				char header[100];
				FBXSDK_sprintf(header, 100, "            Normal Vector: "); 
				if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					// FbxVector4* leNormalsVec = (FbxVector4*) leNormals->GetDirectArray().GetAt(i);
					// FbxVector4* leNormalsVec2 = (FbxVector4*) leNormals2->GetDirectArray().GetAt(i);

					if (i < 100)
					{
						Display3DVector(header, leNormals->GetDirectArray().GetAt(i));
						Display3DVector(header, leNormals2->GetDirectArray().GetAt(i));
					}

					// for (int jj = 0; jj < 3; jj++)
					// {
					// 	leNormalsArray[ii] = leNormalsArray2[ii];
					// }
					leNormals->GetDirectArray().SetAt(i, leNormals2->GetDirectArray().GetAt(i));


					if (i < 100)
					{
						Display3DVector(header, leNormals->GetDirectArray().GetAt(i));
					}
				}
			}
        }
    }

    DisplayString("");
}


// bool ReplacePolygons(FbxMesh* pMesh, FbxMesh* pNewMesh)
// {
//     int i, j, lPolygonCount = pMesh->GetPolygonCount();
//     FbxVector4* lControlPoints = pMesh->GetControlPoints(); 
//     char header[100];

//     // DisplayString("    Polygons");
//     DisplayInt("    Polygons: ", lPolygonCount);

//     int vertexId = 0;
//     for (i = 0; i < lPolygonCount; i++)
//     {
//         DisplayInt("        Polygon ", i);
//         int l;

//         for (l = 0; l < pMesh->GetElementPolygonGroupCount(); l++)
//         {
//             FbxGeometryElementPolygonGroup* lePolgrp = pMesh->GetElementPolygonGroup(l);
// 			switch (lePolgrp->GetMappingMode())
// 			{
// 			case FbxGeometryElement::eByPolygon:
// 				if (lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
// 				{
// 					FBXSDK_sprintf(header, 100, "        Assigned to group: "); 
// 					int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
// 					DisplayInt(header, polyGroupId);
// 					break;
// 				}
// 			default:
// 				// any other mapping modes don't make sense
// 				DisplayString("        \"unsupported group assignment\"");
// 				break;
// 			}
//         }

//         int lPolygonSize = pMesh->GetPolygonSize(i);

// 		for (j = 0; j < lPolygonSize; j++)
// 		{
// 			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
// 			if (lControlPointIndex < 0)
// 			{
// 				DisplayString("            Coordinates: Invalid index found!");
// 				continue;
// 			}
// 			else
// 				Display3DVector("            Coordinates: ", lControlPoints[lControlPointIndex]);

// 			for (l = 0; l < pMesh->GetElementVertexColorCount(); l++)
// 			{
// 				FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor( l);
// 				FBXSDK_sprintf(header, 100, "            Color vertex: "); 

// 				switch (leVtxc->GetMappingMode())
// 				{
// 				default:
// 				    break;
// 				case FbxGeometryElement::eByControlPoint:
// 					switch (leVtxc->GetReferenceMode())
// 					{
// 					case FbxGeometryElement::eDirect:
// 						ReplaceColor(header, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
// 						break;
// 					case FbxGeometryElement::eIndexToDirect:
// 						{
// 							int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
// 							ReplaceColor(header, leVtxc->GetDirectArray().GetAt(id));
// 						}
// 						break;
// 					default:
// 						break; // other reference modes not shown here!
// 					}
// 					break;

// 				case FbxGeometryElement::eByPolygonVertex:
// 					{
// 						switch (leVtxc->GetReferenceMode())
// 						{
// 						case FbxGeometryElement::eDirect:
// 							ReplaceColor(header, leVtxc->GetDirectArray().GetAt(vertexId));
// 							break;
// 						case FbxGeometryElement::eIndexToDirect:
// 							{
// 								int id = leVtxc->GetIndexArray().GetAt(vertexId);
// 								ReplaceColor(header, leVtxc->GetDirectArray().GetAt(id));
// 							}
// 							break;
// 						default:
// 							break; // other reference modes not shown here!
// 						}
// 					}
// 					break;

// 				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
// 				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
// 				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
// 					break;
// 				}
// 			}
// 			for (l = 0; l < pMesh->GetElementUVCount(); ++l)
// 			{
// 				FbxGeometryElementUV* leUV = pMesh->GetElementUV( l);
// 				FBXSDK_sprintf(header, 100, "            Texture UV: "); 

// 				switch (leUV->GetMappingMode())
// 				{
// 				default:
// 				    break;
// 				case FbxGeometryElement::eByControlPoint:
// 					switch (leUV->GetReferenceMode())
// 					{
// 					case FbxGeometryElement::eDirect:
// 						Replace2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
// 						break;
// 					case FbxGeometryElement::eIndexToDirect:
// 						{
// 							int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
// 							Replace2DVector(header, leUV->GetDirectArray().GetAt(id));
// 						}
// 						break;
// 					default:
// 						break; // other reference modes not shown here!
// 					}
// 					break;

// 				case FbxGeometryElement::eByPolygonVertex:
// 					{
// 						int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
// 						switch (leUV->GetReferenceMode())
// 						{
// 						case FbxGeometryElement::eDirect:
// 						case FbxGeometryElement::eIndexToDirect:
// 							{
// 								Replace2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));
// 							}
// 							break;
// 						default:
// 							break; // other reference modes not shown here!
// 						}
// 					}
// 					break;

// 				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
// 				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
// 				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
// 					break;
// 				}
// 			}
// 			for( l = 0; l < pMesh->GetElementNormalCount(); ++l)
// 			{
// 				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal( l);
// 				FBXSDK_sprintf(header, 100, "            Normal: "); 

// 				if(leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
// 				{
// 					switch (leNormal->GetReferenceMode())
// 					{
// 					case FbxGeometryElement::eDirect:
// 						Display3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));
// 						break;
// 					case FbxGeometryElement::eIndexToDirect:
// 						{
// 							int id = leNormal->GetIndexArray().GetAt(vertexId);
// 							Display3DVector(header, leNormal->GetDirectArray().GetAt(id));
// 						}
// 						break;
// 					default:
// 						break; // other reference modes not shown here!
// 					}
// 				}

// 			}
// 			for( l = 0; l < pMesh->GetElementTangentCount(); ++l)
// 			{
// 				FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent( l);
// 				FBXSDK_sprintf(header, 100, "            Tangent: "); 

// 				if(leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
// 				{
// 					switch (leTangent->GetReferenceMode())
// 					{
// 					case FbxGeometryElement::eDirect:
// 						Display3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
// 						break;
// 					case FbxGeometryElement::eIndexToDirect:
// 						{
// 							int id = leTangent->GetIndexArray().GetAt(vertexId);
// 							Display3DVector(header, leTangent->GetDirectArray().GetAt(id));
// 						}
// 						break;
// 					default:
// 						break; // other reference modes not shown here!
// 					}
// 				}

// 			}
// 			for( l = 0; l < pMesh->GetElementBinormalCount(); ++l)
// 			{

// 				FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal( l);

// 				FBXSDK_sprintf(header, 100, "            Binormal: "); 
// 				if(leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
// 				{
// 					switch (leBinormal->GetReferenceMode())
// 					{
// 					case FbxGeometryElement::eDirect:
// 						Display3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));
// 						break;
// 					case FbxGeometryElement::eIndexToDirect:
// 						{
// 							int id = leBinormal->GetIndexArray().GetAt(vertexId);
// 							Display3DVector(header, leBinormal->GetDirectArray().GetAt(id));
// 						}
// 						break;
// 					default:
// 						break; // other reference modes not shown here!
// 					}
// 				}
// 			}
// 			vertexId++;
// 		} // for polygonSize
//     } // for polygonCount


//     //check visibility for the edges of the mesh
// 	for(int l = 0; l < pMesh->GetElementVisibilityCount(); ++l)
// 	{
// 		FbxGeometryElementVisibility* leVisibility=pMesh->GetElementVisibility(l);
// 		FBXSDK_sprintf(header, 100, "    Edge Visibility : ");
// 		DisplayString(header);
// 		switch(leVisibility->GetMappingMode())
// 		{
// 		default:
// 		    break;
// 			//should be eByEdge
// 		case FbxGeometryElement::eByEdge:
// 			//should be eDirect
// 			for(j=0; j!=pMesh->GetMeshEdgeCount();++j)
// 			{
// 				DisplayInt("        Edge ", j);
// 				ReplaceBool("              Edge visibility: ", leVisibility->GetDirectArray().GetAt(j));
// 			}

// 			break;
// 		}
// 	}
//     DisplayString("");
// }

// bool ReplaceTextureNames( FbxProperty &pProperty, FbxString& pConnectionString )
// {
//     int lLayeredTextureCount = pProperty.GetSrcObjectCount<FbxLayeredTexture>();
//     if(lLayeredTextureCount > 0)
//     {
//         for(int j=0; j<lLayeredTextureCount; ++j)
//         {
//             FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
//             int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
//             pConnectionString += " Texture ";

//             for(int k =0; k<lNbTextures; ++k)
//             {
//                 //lConnectionString += k;
//                 pConnectionString += "\"";
//                 pConnectionString += (char*)lLayeredTexture->GetName();
//                 pConnectionString += "\"";
//                 pConnectionString += " ";
//             }
//             pConnectionString += "of ";
//             pConnectionString += pProperty.GetName();
//             pConnectionString += " on layer ";
//             pConnectionString += j;
//         }
//         pConnectionString += " |";
//     }
//     else
//     {
//         //no layered texture simply get on the property
//         int lNbTextures = pProperty.GetSrcObjectCount<FbxTexture>();

//         if(lNbTextures > 0)
//         {
//             pConnectionString += " Texture ";
//             pConnectionString += " ";

//             for(int j =0; j<lNbTextures; ++j)
//             {
//                 FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
//                 if(lTexture)
//                 {
//                     pConnectionString += "\"";
//                     pConnectionString += (char*)lTexture->GetName();
//                     pConnectionString += "\"";
//                     pConnectionString += " ";
//                 }
//             }
//             pConnectionString += "of ";
//             pConnectionString += pProperty.GetName();
//             pConnectionString += " |";
//         }
//     }
// }

// bool ReplaceMaterialTextureConnections( FbxSurfaceMaterial* pMaterial, char * header, int pMatId, int l )
// {
//     if(!pMaterial)
//         return;

//     FbxString lConnectionString = "            Material %d -- ";
//     //Show all the textures

//     FbxProperty lProperty;
//     //Diffuse Textures
//     lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
//     ReplaceTextureNames(lProperty, lConnectionString);

//     //DiffuseFactor Textures
//     lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
//     ReplaceTextureNames(lProperty, lConnectionString);

//     //Emissive Textures
//     lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);
//     ReplaceTextureNames(lProperty, lConnectionString);

//     //EmissiveFactor Textures
//     lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissiveFactor);
//     ReplaceTextureNames(lProperty, lConnectionString);


//     //Ambient Textures
//     lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbient);
//     ReplaceTextureNames(lProperty, lConnectionString); 

//     //AmbientFactor Textures
//     lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbientFactor);
//     ReplaceTextureNames(lProperty, lConnectionString);          

//     //Specular Textures
//     lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
//     ReplaceTextureNames(lProperty, lConnectionString);  

//     //SpecularFactor Textures
//     lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecularFactor);
//     ReplaceTextureNames(lProperty, lConnectionString);

//     //Shininess Textures
//     lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
//     ReplaceTextureNames(lProperty, lConnectionString);

//     //Bump Textures
//     lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sBump);
//     ReplaceTextureNames(lProperty, lConnectionString);

//     //Normal Map Textures
//     lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
//     ReplaceTextureNames(lProperty, lConnectionString);

//     //Transparent Textures
//     lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparentColor);
//     ReplaceTextureNames(lProperty, lConnectionString);

//     //TransparencyFactor Textures
//     lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
//     ReplaceTextureNames(lProperty, lConnectionString);

//     //Reflection Textures
//     lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sReflection);
//     ReplaceTextureNames(lProperty, lConnectionString);

//     //ReflectionFactor Textures
//     lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sReflectionFactor);
//     ReplaceTextureNames(lProperty, lConnectionString);

//     //Update header with material info
// 	bool lStringOverflow = (lConnectionString.GetLen() + 10 >= MAT_HEADER_LENGTH); // allow for string length and some padding for "%d"
// 	if (lStringOverflow)
// 	{
// 		// Truncate string!
// 		lConnectionString = lConnectionString.Left(MAT_HEADER_LENGTH - 10);
// 		lConnectionString = lConnectionString + "...";
// 	}
// 	FBXSDK_sprintf(header, MAT_HEADER_LENGTH, lConnectionString.Buffer(), pMatId, l);
// 	DisplayString(header);
// }

// bool ReplaceMaterialConnections(FbxMesh* pMesh, FbxMesh* pNewMesh)
// {
//     int i, l, lPolygonCount = pMesh->GetPolygonCount();

//     char header[MAT_HEADER_LENGTH];

//     DisplayString("    Polygons Material Connections");

//     //check whether the material maps with only one mesh
//     bool lIsAllSame = true;
//     for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
//     {

//         FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
// 		if( lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon) 
// 		{
// 			lIsAllSame = false;
// 			break;
// 		}
//     }

//     //For eAllSame mapping type, just out the material and texture mapping info once
//     if(lIsAllSame)
//     {
//         for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
//         {

//             FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial( l);
// 			if( lMaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame) 
// 			{
// 				FbxSurfaceMaterial* lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(0));    
// 				int lMatId = lMaterialElement->GetIndexArray().GetAt(0);
// 				if(lMatId >= 0)
// 				{
// 					DisplayInt("        All polygons share the same material in mesh ", l);
// 					ReplaceMaterialTextureConnections(lMaterial, header, lMatId, l);
// 				}
// 			}
//         }

// 		//no material
// 		if(l == 0)
// 			DisplayString("        no material applied");
//     }

//     //For eByPolygon mapping type, just out the material and texture mapping info once
//     else
//     {
//         for (i = 0; i < lPolygonCount; i++)
//         {
//             DisplayInt("        Polygon ", i);

//             for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
//             {

//                 FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial( l);
// 				FbxSurfaceMaterial* lMaterial = NULL;
// 				int lMatId = -1;
// 				lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(i));
// 				lMatId = lMaterialElement->GetIndexArray().GetAt(i);

// 				if(lMatId >= 0)
// 				{
// 					ReplaceMaterialTextureConnections(lMaterial, header, lMatId, l);
// 				}
//             }
//         }
//     }
// }


// bool ReplaceMaterialMapping(FbxMesh* pMesh, FbxMesh* pNewMesh)
// {
//     const char* lMappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "By Edge", "All Same" };
//     const char* lReferenceMode[] = { "Direct", "Index", "Index to Direct"};

//     int lMtrlCount = 0;
//     FbxNode* lNode = NULL;
//     if(pMesh){
//         lNode = pMesh->GetNode();
//         if(lNode)
//             lMtrlCount = lNode->GetMaterialCount();    
//     }

//     for (int l = 0; l < pMesh->GetElementMaterialCount(); l++)
//     {
//         FbxGeometryElementMaterial* leMat = pMesh->GetElementMaterial( l);
//         if (leMat)
//         {
//             char header[100];
//             FBXSDK_sprintf(header, 100, "    Material Element %d: ", l); 
//             DisplayString(header);


//             DisplayString("           Mapping: ", lMappingTypes[leMat->GetMappingMode()]);
//             DisplayString("           ReferenceMode: ", lReferenceMode[leMat->GetReferenceMode()]);

//             int lMaterialCount = 0;
//             FbxString lString;

//             if (leMat->GetReferenceMode() == FbxGeometryElement::eDirect ||
//                 leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
//             {
//                 lMaterialCount = lMtrlCount;
//             }

//             if (leMat->GetReferenceMode() == FbxGeometryElement::eIndex ||
//                 leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
//             {
//                 int i;

//                 lString = "           Indices: ";

//                 int lIndexArrayCount = leMat->GetIndexArray().GetCount(); 
//                 for (i = 0; i < lIndexArrayCount; i++)
//                 {
//                     lString += leMat->GetIndexArray().GetAt(i);

//                     if (i < lIndexArrayCount - 1)
//                     {
//                         lString += ", ";
//                     }
//                 }

//                 lString += "\n";

//                 FBXSDK_printf(lString);
//             }
//         }
//     }

//     DisplayString("");
// }
