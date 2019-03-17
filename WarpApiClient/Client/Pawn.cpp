#include "stdafx.h"
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

void Pawn::MoveWithDirection(DIRECTION inDirection)
{
	switch (inDirection)
	{
	case DIRECTION::UP:
		if (yPosition != BLOCK_MIN_POSITION) --yPosition;
		break;
	case DIRECTION::DOWN:
		if (yPosition != BLOCK_MAX_POSITION) ++yPosition;
		break;
	case DIRECTION::LEFT:
		if (xPosition != BLOCK_MIN_POSITION) --xPosition;
		break;
	case DIRECTION::RIGHT:
		if (xPosition != BLOCK_MAX_POSITION) ++xPosition;
		break;
	}

	UpdateRenderData();
}

void Pawn::SetPosition(const std::pair<UINT8, UINT8> inPosition)
{
	xPosition = inPosition.first;
	yPosition = inPosition.second;

	UpdateRenderData();
}

void Pawn::UpdateRenderData()
{
	renderData->xPosition = xPosition * BLOCK_WIDTH_SIZE;
	renderData->yPosition = yPosition * BLOCK_HEIGHT_SIZE;
}
