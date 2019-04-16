#include "stdafx.h"
#include "ClientDefine.h"

#include "BaseModel.h"

#include "Pawn.h"

Pawn::Pawn(BaseModel* const inBaseModel, const RenderData& inRenderData,
	const UINT8 inXPosition, const UINT8 inYPosition)
	: BaseActor(inBaseModel, inRenderData)
	, xPosition(inXPosition), yPosition(inYPosition)
{
}

Pawn::~Pawn()
{
}

//void Pawn::MoveWithDirection(DIRECTION inDirection)
//{
//	switch (inDirection)
//	{
//	case DIRECTION::UP:
//		if (yPosition != BLOCK_MIN_POSITION) --yPosition;
//		break;
//	case DIRECTION::DOWN:
//		if (yPosition != BLOCK_MAX_POSITION) ++yPosition;
//		break;
//	case DIRECTION::LEFT:
//		if (xPosition != BLOCK_MIN_POSITION) --xPosition;
//		break;
//	case DIRECTION::RIGHT:
//		if (xPosition != BLOCK_MAX_POSITION) ++xPosition;
//		break;
//	}
//
//	UpdateRenderData();
//}

void Pawn::Render(HDC pHDC) 
{
	if (isRender) 
	{
		// ÄÃ¸µ
		if (renderData->xPosition <= -3000) return;
		if (renderData->xPosition >= 1100) return;

		if (renderData->yPosition <= -3000) return;
		if (renderData->yPosition >= 1100) return;
		
		renderComponent->Render(pHDC, renderData);
	}
}

void Pawn::SetPosition(const std::pair<UINT8, UINT8> inPosition, const std::pair<UINT8, UINT8> inMainPlayerPosition)
{
	xPosition = inPosition.first;
	yPosition = inPosition.second;

	UpdateRenderData(inMainPlayerPosition);
}

void Pawn::SetOnlyActorPositionNotUpdateRenderData(const std::pair<UINT8, UINT8> inPosition)
{
	xPosition = inPosition.first;
	yPosition = inPosition.second;
}

void Pawn::UpdateRenderData(const std::pair<UINT8, UINT8> inMainPlayerPosition)
{
	renderData->xPosition = (xPosition - inMainPlayerPosition.first) * GLOBAL_DEFINE::BLOCK_WIDTH_SIZE + GLOBAL_DEFINE::PLAY_FRAME_HALF_WIDTH;
	renderData->yPosition = (yPosition - inMainPlayerPosition.second) * GLOBAL_DEFINE::BLOCK_HEIGHT_SIZE + GLOBAL_DEFINE::PLAY_FRAME_HALF_HEIGHT;
}
