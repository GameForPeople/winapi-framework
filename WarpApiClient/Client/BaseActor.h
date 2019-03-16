#pragma once

class BaseModel;
struct RenderData;

class BaseActor {
public:
	BaseActor(BaseModel* const inBaseModel) noexcept;
	BaseActor(BaseModel* const inBaseModel, const RenderData& inRenderData) noexcept;
	virtual ~BaseActor();

public:
	void Render(HDC pHDC);

protected:
	BaseModel* renderComponent;
	RenderData* renderData;
};