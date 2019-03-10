#include "stdafx.h"

#include "../RenderModel/BaseModel.h"
#include "../Object/BaseActor.h"

BaseActor::BaseActor(BaseModel* const inBaseModel) noexcept
	: renderComponent(inBaseModel), renderData()
{
}

BaseActor::BaseActor(BaseModel* const inBaseModel, const RenderData& inRenderData) noexcept
	: renderComponent(inBaseModel), renderData(new RenderData(inRenderData))
{
}

BaseActor::~BaseActor()
{
}

void BaseActor::Render(HDC pHDC)
{
	renderComponent->Render(pHDC, renderData);
}