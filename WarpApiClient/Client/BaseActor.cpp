#include "stdafx.h"

#include "BaseModel.h"
#include "BaseActor.h"

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