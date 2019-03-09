#pragma once

#include "BaseModel.h"

class TransparentModel final : public BaseModel
{
public:
	TransparentModel(const std::wstring& inResourcePath);
	virtual ~TransparentModel() override final = default;

public:
	virtual void Render(HDC pHDC, RenderData* inPoint) override final;
};