#include "stdafx.h"

#include "BaseModel.h"
#include "BaseActor.h"

BaseActor::BaseActor(BaseModel* const inBaseModel) noexcept
	: renderComponent(inBaseModel), renderData(), isRender(true)
{
}

BaseActor::BaseActor(BaseModel* const inBaseModel, const RenderData& inRenderData, bool inIsRender /* = true*/) noexcept
	: renderComponent(inBaseModel), renderData(new RenderData(inRenderData)), isRender(inIsRender)
{
}

BaseActor::~BaseActor()
{
}

void BaseActor::Render(HDC pHDC)
{
	if(isRender) renderComponent->Render(pHDC, renderData);
}