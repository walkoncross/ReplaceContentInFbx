/****************************************************************************************
// Based on <FBX_SDK>/samples/ImportScene/DisplayShape.h
// author: zhaoyafei0210@gmail.com
****************************************************************************************/

#include "DisplayCommon.h"

#include <fbxsdk.h>

void DisplayElementData(const FbxString& pHeader, const FbxVector4& pData, int index = -1);

// void ReplaceShapeLayerElements(const FbxShape* pShape, const FbxMesh* pMesh);
bool ReplaceShapeLayerElements(FbxShape* pShape, const FbxMesh* pMesh, const FbxShape* pShape2, const FbxMesh* pMesh2);

bool ReplaceShape(FbxGeometry* pGeometry, FbxGeometry* pNewGeometry)
{
	bool lResult = false;

	int lBlendShapeCount, lBlendShapeChannelCount, lTargetShapeCount;
	FbxBlendShape* lBlendShape;
	FbxBlendShapeChannel* lBlendShapeChannel;
	FbxShape* lShape;

	int lBlendShapeCount2, lBlendShapeChannelCount2, lTargetShapeCount2;
	FbxBlendShape* lBlendShape2 = NULL;
	FbxBlendShapeChannel* lBlendShapeChannel2 = NULL;
	FbxShape* lShape2 = NULL;

	if (!pGeometry || !pNewGeometry)
	{
		return false;
	}
	
	lBlendShapeCount = pGeometry->GetDeformerCount(FbxDeformer::eBlendShape);

	DisplayInt("    BlendShapeCount: ", lBlendShapeCount);

	lBlendShapeCount2 = pNewGeometry->GetDeformerCount(FbxDeformer::eBlendShape);
	DisplayInt("    BlendShapeCount2: ", lBlendShapeCount2);

	DisplayString("");

	for (int lBlendShapeIndex = 0; lBlendShapeIndex < lBlendShapeCount; ++lBlendShapeIndex)
	{
		lBlendShape = (FbxBlendShape*)pGeometry->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);
		DisplayString("    BlendShape ", (char *)lBlendShape->GetName());

		lBlendShape2 = NULL;

		for(int ii = 0; ii < lBlendShapeCount2; ++ii)
		{
			FbxBlendShape* lBlendShape2_t = (FbxBlendShape*)pNewGeometry->GetDeformer(ii, FbxDeformer::eBlendShape);
			if (!strcmp((char *)lBlendShape->GetName(), (char *)lBlendShape2_t->GetName()))
			{
				lBlendShape2 = lBlendShape2_t;
				DisplayString("    Found blend shape with same name");
				break;
			}
		}
		
		lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
		DisplayInt("    BlendShapeChannelCount: ", lBlendShapeChannelCount);

		lBlendShapeChannelCount2 = 0;

		if (/* condition */ lBlendShape2)
		{
			lBlendShapeChannelCount2 = lBlendShape2->GetBlendShapeChannelCount();
			DisplayInt("    BlendShapeChannelCount2: ", lBlendShapeChannelCount2);
		}

		for (int lBlendShapeChannelIndex = 0; lBlendShapeChannelIndex < lBlendShapeChannelCount; ++lBlendShapeChannelIndex)
		{
			lBlendShapeChannel = lBlendShape->GetBlendShapeChannel(lBlendShapeChannelIndex);
			DisplayString("    BlendShapeChannel ", (char *)lBlendShapeChannel->GetName());
			DisplayDouble("    Default Deform Value: ", lBlendShapeChannel->DeformPercent.Get());

			lBlendShapeChannel2 = NULL;
			if (lBlendShapeChannelCount2 > 0)
			{
				for(int ii = 0; ii < lBlendShapeChannelCount2; ++ii)
				{
					FbxBlendShapeChannel* lBlendShapeChannel2_t = lBlendShape->GetBlendShapeChannel(lBlendShapeChannelIndex);
					if (!!strcmp((char *)lBlendShapeChannel->GetName(), (char *)lBlendShapeChannel2_t->GetName()))
					{
						lBlendShapeChannel2 = lBlendShapeChannel2_t;
						DisplayString("    Found blend shape channel with same name");

						break;
					}					
				}
			}

			lTargetShapeCount = lBlendShapeChannel->GetTargetShapeCount();

			lTargetShapeCount2 = 0;
			if (lBlendShapeChannel2) lTargetShapeCount2 = lBlendShapeChannel2->GetTargetShapeCount();

			for (int lTargetShapeIndex = 0; lTargetShapeIndex < lTargetShapeCount; ++lTargetShapeIndex)
			{
				lShape = lBlendShapeChannel->GetTargetShape(lTargetShapeIndex);
				DisplayString("    TargetShape ", (char *)lShape->GetName());
				DisplayInt("    TargetShapeIndex: ", lTargetShapeIndex);

				lShape2 = NULL;

				if (lTargetShapeCount2 > 0)
				{
					for(int ii = 0; ii<lTargetShapeCount2; ++ii)
					{
						FbxShape* lShape2_t = lBlendShapeChannel->GetTargetShape(lTargetShapeIndex);
						if (!!strcmp((char *)lShape->GetName(), (char *)lShape2_t->GetName()))
						{
							lShape2 = lShape2_t;
							DisplayString("    Found target shape with same name");
							break;
						}
					}
				}

				if (pGeometry->GetAttributeType() == FbxNodeAttribute::eMesh)
				{
					lResult = ReplaceShapeLayerElements(lShape, FbxCast<FbxMesh>(pGeometry), lShape2, FbxCast<FbxMesh>(pNewGeometry));
				}
				// else
				// {
				// 	int j, lControlPointsCount = lShape->GetControlPointsCount();
				// 	FbxVector4* lControlPoints = lShape->GetControlPoints();
				// 	FbxLayerElementArrayTemplate<FbxVector4>* lNormals = NULL;
				// 	bool lStatus = lShape->GetNormals(&lNormals);

				// 	DisplayInt("    ControlPointsCount:", lControlPointsCount);

				// 	for (j = 0; j < lControlPointsCount; j++)
				// 	{
				// 		DisplayInt("        Control Point ", j);
				// 		Display3DVector("            Coordinates: ", lControlPoints[j]);

				// 		if (lStatus && lNormals && lNormals->GetCount() == lControlPointsCount)
				// 		{
				// 			Display3DVector("            Normal Vector: ", lNormals->GetAt(j));
				// 		}
				// 	}
				// }

				DisplayString("");
			}
		}
	}

	return lResult;
}

// void DisplayElementData(const FbxString& pHeader, const FbxVector4& pData, int index = -1)
// {
//     FbxString desc(pHeader);
//     if (index != -1)
//     {
//         FbxString num = FbxString(" [") + index + "]: ";
//         desc.FindAndReplace(":", num.Buffer());
//     }
//     Display3DVector(desc.Buffer(), pData);
// }

// void DisplayElementData(const FbxString& pHeader, const FbxVector2& pData, int index = -1)
// {
//     FbxString desc(pHeader);
//     if (index != -1)
//     {
//         FbxString num = FbxString(" [") + index + "]: ";
//         desc.FindAndReplace(":", num.Buffer());
//     }
// 	Display2DVector(desc.Buffer(), pData);
// }

// void DisplayElementData(const FbxString&  pHeader, const FbxColor& pData, int index = -1)
// {
//     FbxString desc(pHeader);
//     if (index != -1)
//     {
//         FbxString num = FbxString(" [") + index + "]: ";
//         desc.FindAndReplace(":", num.Buffer());
//     }
// 	DisplayColor(desc.Buffer(), pData);
// }

// void FillHeaderBasedOnElementType(FbxLayerElement::EType pComponent, FbxString& pHeader)
// {
// 	switch (pComponent)
// 	{
// 	case FbxLayerElement::eNormal:
// 		pHeader = "        Normal: ";
// 		break;
// 	case FbxLayerElement::eBiNormal:
// 		pHeader = "        BiNormal: ";
// 		break;
// 	case FbxLayerElement::eTangent:
// 		pHeader = "        Tangent: ";
// 		break;
// 	case FbxLayerElement::eUV:
// 		pHeader = "        UV: ";
// 		break;
// 	case FbxLayerElement::eVertexColor:
// 		pHeader = "        Vertex Color: ";
// 		break;
// 	default:
// 		pHeader = "        Unsupported element: ";
// 		break;
// 	}
// }

// template<class T>
// void DisplayLayerElement(FbxLayerElement::EType pComponent, const FbxLayer* pShapeLayer, const FbxMesh* pMesh)
// {
// 	const FbxLayerElement* pLayerElement = pShapeLayer->GetLayerElementOfType(pComponent);
// 	if (pLayerElement)
// 	{
// 		FbxLayerElementTemplate<T>* pLayerElementTemplate = ((FbxLayerElementTemplate<T>*) pLayerElement);
// 		FbxLayerElementArrayTemplate<T>& pLayerElementArray = pLayerElementTemplate->GetDirectArray();		
// 		FbxString header;
// 		FillHeaderBasedOnElementType(pComponent, header);
// 		int lPolygonCount = pMesh->GetPolygonCount();
// 		int lPolynodeIndex = 0;
// 		for (int i = 0; i < lPolygonCount; ++i)
// 		{
// 			int lPolygonSize = pMesh->GetPolygonSize(i);
// 			for (int j = 0; j < lPolygonSize; ++j, ++lPolynodeIndex)
// 			{
// 				int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
// 				switch (pLayerElementTemplate->GetMappingMode())
// 				{
// 				default:
// 					break;
// 				case FbxGeometryElement::eByControlPoint:
// 				{
// 					switch (pLayerElementTemplate->GetReferenceMode())
// 					{
// 					case FbxGeometryElement::eDirect:
// 						DisplayElementData(header, pLayerElementArray.GetAt(lControlPointIndex), lControlPointIndex);
// 						break;
// 					case FbxGeometryElement::eIndexToDirect:
// 					{
//                         FbxLayerElementArrayTemplate<int>& pLayerElementIndexArray = pLayerElementTemplate->GetIndexArray();
// 						int id = pLayerElementIndexArray.GetAt(lControlPointIndex);
// 						if (id > 0) { DisplayElementData(header, pLayerElementArray.GetAt(id), id); }
// 					}
// 					break;
// 					default:
// 						break; // other reference modes not shown here!
// 					};
// 				}
// 				case FbxGeometryElement::eByPolygonVertex:
// 				{
// 					switch (pLayerElementTemplate->GetReferenceMode())
// 					{
// 					case FbxGeometryElement::eDirect:
// 						DisplayElementData(header, pLayerElementArray.GetAt(lPolynodeIndex), lPolynodeIndex);
// 						break;
// 					case FbxGeometryElement::eIndexToDirect:
// 					{
//                         FbxLayerElementArrayTemplate<int>& pLayerElementIndexArray = pLayerElementTemplate->GetIndexArray();
// 						int id = pLayerElementIndexArray.GetAt(lPolynodeIndex);
// 						if (id > 0) { DisplayElementData(header, pLayerElementArray.GetAt(id), id); }
// 					}
// 					break;
// 					default:
// 						break; // other reference modes not shown here!
// 					}
// 				}
// 				break;
// 				}
// 			}
// 		}
// 	}
// }

bool ReplaceShapeLayerElements(FbxShape* pShape, const FbxMesh* pMesh, const FbxShape* pShape2, const FbxMesh* pMesh2)
{
    int lShapeControlPointsCount = 0;
    int lMeshControlPointsCount = 0;

    bool lValidLayerElementSource = false;

	bool lResult = false;

	if (!pShape || !pMesh || !pMesh2)
	{
		return false;
	}
	
	lShapeControlPointsCount = pShape->GetControlPointsCount();
    lMeshControlPointsCount = pMesh->GetControlPointsCount();

    lValidLayerElementSource = lShapeControlPointsCount && lMeshControlPointsCount && (lShapeControlPointsCount == lMeshControlPointsCount);

	if (lValidLayerElementSource)
	{
		// Display control points that are different from the mesh
		FbxVector4* lShapeControlPoint = pShape->GetControlPoints();
		FbxVector4* lMeshControlPoint = pMesh->GetControlPoints();

        if (pShape2 && pShape2->GetControlPointsCount() == lShapeControlPointsCount)
        {
            // set control points
			for (int j = 0; j < lShapeControlPointsCount; j++)
			{
                if (j<100)
                {
                    FbxString header("        Control Point: ");
                    DisplayElementData(header, lShapeControlPoint[j], j);
                }

				pShape->SetControlPointAt(pShape2->GetControlPointAt(j), j);

                if (j<100)
                {
                    FbxString header("        Control Point: ");
                    DisplayElementData(header, lShapeControlPoint[j], j);
                }
			}

			lResult = true;
        }
		else if (pMesh2 && pMesh2->GetControlPointsCount() == lShapeControlPointsCount)
		{
            // Display control points that are different from the mesh
            FbxVector4* lMeshControlPoint2 = pMesh2->GetControlPoints();

			int cnt = 0;
			
			for (int j = 0; j < lShapeControlPointsCount; j++)
			{				
				FbxVector4 delta = lMeshControlPoint2[j] - lMeshControlPoint[j];
				// FbxVector4 newControlPoint = lShapeControlPoint[j] - lMeshControlPoint[j] + lMeshControlPoint2[j];
				FbxVector4 newControlPoint = lShapeControlPoint[j] + delta;

				// if (j<100)
                if (!FbxEqual(delta, FbxZeroVector4) && cnt < 20)
				{
					FbxString header("        Control Point: ");
                    DisplayElementData(header, lShapeControlPoint[j], j);
				}

				pShape->SetControlPointAt(newControlPoint, j);

				// if (j<100)
                if (!FbxEqual(delta, FbxZeroVector4) && cnt < 20)
                {
                    FbxString header("        Control Point: ");
                    DisplayElementData(header, lShapeControlPoint[j], j);

					cnt++;
                }
			}		

			lResult = true;
		}
		
		// int lLayerCount = pShape->GetLayerCount();
		// for (int i = 0; i < lLayerCount; ++i)
		// {
		// 	const FbxLayer *pLayer = pShape->GetLayer(i);
		// 	DisplayLayerElement<FbxVector4>(FbxLayerElement::eNormal, pLayer, pMesh);
		// 	DisplayLayerElement<FbxColor>(FbxLayerElement::eVertexColor, pLayer, pMesh);
		// 	DisplayLayerElement<FbxVector4>(FbxLayerElement::eTangent, pLayer, pMesh);
		// 	DisplayLayerElement<FbxColor>(FbxLayerElement::eBiNormal, pLayer, pMesh);
		// 	DisplayLayerElement<FbxColor>(FbxLayerElement::eUV, pLayer, pMesh);
		// }
	}

	return lResult;
}

