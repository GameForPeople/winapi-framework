#include "stdafx.h"
#include "ClientDefine.h"

#include "BaseModel.h"
#include "BitModel.h"
#include "StretchModel.h"
#include "TransparentModel.h"

#include "RenderModelManager.h"

RenderModelManager::RenderModelManager()
{
	const BYTE renderModelCount = static_cast<BYTE>(RENDER_MODEL_TYPE::ENUM_SIZE);
	renderModelCont.reserve(renderModelCount);

	// ���� ������ �����ϰ�...
	for (int i = 0; i < renderModelCount; ++i) { renderModelCont.emplace_back(nullptr); }
	
	renderModelCont[static_cast<BYTE>(RENDER_MODEL_TYPE::PLAYER)] = new TransparentModel(L"Resource/Image/Image_PlayerCharacter.png");
	renderModelCont[static_cast<BYTE>(RENDER_MODEL_TYPE::OTHER_PLAYER)] = new TransparentModel(L"Resource/Image/Image_OtherCharacter.png");
	renderModelCont[static_cast<BYTE>(RENDER_MODEL_TYPE::BACKGROUND)] = new StretchModel(L"Resource/Image/Image_New_Background.png");
	renderModelCont[static_cast<BYTE>(RENDER_MODEL_TYPE::COVER_UI)] = new StretchModel(L"Resource/Image/Image_Cover.png");
	renderModelCont[static_cast<BYTE>(RENDER_MODEL_TYPE::BROADCAST_UI)] = new TransparentModel(L"Resource/Image/Image_BroadcastArea.png");

	assert(
		[/* void */](const std::vector<BaseModel*>& inRenderModelCont) noexcept -> bool 
		{
			if (inRenderModelCont.size() != static_cast<BYTE>(RENDER_MODEL_TYPE::ENUM_SIZE))
				return false;

			for (const auto& pRenderModel : inRenderModelCont)
			{
				if (pRenderModel == nullptr)
					return false;
			}

			return true;
		}(renderModelCont),
		"�������� ���� RenderModel�� �ֽ��ϴ�. Ȯ�����ּ���. Ŭ���̾�Ʈ�� �����մϴ�."
	);
}

RenderModelManager::~RenderModelManager()
{
	for (auto pRenderModel : renderModelCont)
	{
		delete pRenderModel;
	}
}

BaseModel* RenderModelManager::GetRenderModel(RENDER_MODEL_TYPE inRenderModelType)
{
	return renderModelCont[static_cast<BYTE>(inRenderModelType)];
}