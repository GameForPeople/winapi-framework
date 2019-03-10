#include "stdafx.h"

#include "../RenderModel/TransparentModel.h"

TransparentModel::TransparentModel(const std::wstring& inResourcePath)
	: BaseModel(inResourcePath)
{
}

void TransparentModel::Render(HDC pHDC, RenderData* inRenderData)
{
	imageOfModel->TransparentBlt(
		pHDC,
		inRenderData->/*transparentData.*/xPosition,
		inRenderData->/*transparentData.*/yPosition,
		inRenderData->/*transparentData.*/xSize,
		inRenderData->/*transparentData.*/ySize,
		inRenderData->/*transparentData.*/rgbColor
	);
}