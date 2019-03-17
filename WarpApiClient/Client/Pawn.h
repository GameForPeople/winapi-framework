#pragma once

#include "BaseActor.h"

class BaseModel;

class Pawn : public BaseActor
{
public:
	Pawn(BaseModel* const inBaseModel, const RenderData& inRenderData,
	const UINT8 inXPosition = 0, const UINT8 inYPosition = 0);
	virtual ~Pawn() override;

public:
	void MoveWithDirection(DIRECTION inDirection);
	void SetPosition(const std::pair<UINT8, UINT8>);
	
private:
	void UpdateRenderData();

private:
	UINT8 xPosition;
	UINT8 yPosition;

	static constexpr UINT8 BLOCK_WIDTH_SIZE = 100;
	static constexpr UINT8 BLOCK_HEIGHT_SIZE = 95;

	static constexpr UINT8 BLOCK_MIN_POSITION = 0;
	static constexpr UINT8 BLOCK_MAX_POSITION = 7;
};