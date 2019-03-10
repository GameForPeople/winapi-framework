#include "stdafx.h"

#include "../RenderModel/BaseModel.h"

BaseModel::BaseModel(const std::wstring inResourcePath)
{
	imageOfModel = std::make_unique<CImage>();
	imageOfModel->Load(inResourcePath.c_str());
}

BaseModel::~BaseModel()
{
	imageOfModel->Destroy();
}