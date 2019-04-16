#include "stdafx.h"

#include "BaseModel.h"
#include "BaseActor.h"

BaseActor::BaseActor(BaseModel* const inBaseModel) noexcept
	: renderComponent(inBaseModel), renderData(), isRender(true)
{
	assert(inBaseModel != nullptr, "��ü �ʱ�ȭ����, nullptr�� RenderModel�� ���޵Ǿ����ϴ�.");
}

BaseActor::BaseActor(BaseModel* const inBaseModel, const RenderData& inRenderData, bool inIsRender /* = true*/) noexcept
	: renderComponent(inBaseModel), renderData(new RenderData(inRenderData)), isRender(inIsRender)
{
	assert(inBaseModel != nullptr, "��ü �ʱ�ȭ����, nullptr�� RenderModel�� ���޵Ǿ����ϴ�.");
}

BaseActor::~BaseActor()
{
}

void BaseActor::Render(HDC pHDC)
{
	if(isRender) renderComponent->Render(pHDC, renderData);
}