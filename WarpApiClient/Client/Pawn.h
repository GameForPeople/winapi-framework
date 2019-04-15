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
	virtual void Render(HDC pHDC) override;

	//void MoveWithDirection(DIRECTION inDirection); // only Client
	void SetPosition(const std::pair<UINT8, UINT8>, const std::pair<UINT8, UINT8>);
	void UpdateRenderData(const std::pair<UINT8, UINT8>);
	inline std::pair<UINT8, UINT8> GetPosition() noexcept { return std::make_pair(xPosition, yPosition); }

private:
	UINT8 xPosition;
	UINT8 yPosition;

	static constexpr UINT8 BLOCK_WIDTH_SIZE = 70;
	static constexpr UINT8 BLOCK_HEIGHT_SIZE = 70;

	//static constexpr UINT8 BLOCK_MIN_POSITION = 0;
	//static constexpr UINT8 BLOCK_MAX_POSITION = 7;
};