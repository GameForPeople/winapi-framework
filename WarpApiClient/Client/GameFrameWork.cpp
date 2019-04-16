/*
	kpu-warp-winapi-framework		Copyright �� https://github.com/KPU-WARP

	#0. �ش� ������Ʈ�� �ѱ��������� ���Ӱ��а� �м� �Ҹ��� WARP�� ���¼ҽ� WinAPI Framework�Դϴ�.
	#1. ���� ���� ������ ���� ����� ����̿��� Ȯ���Ͻ� �� �ֽ��ϴ�.
		- https://github.com/KPU-WARP/winapi-framework

	#2. ��Ÿ ���Ǵ�, KoreaGameMaker@gmail.com���� ��Ź�帳�ϴ�. �����մϴ� :)
*/

#include "stdafx.h"

#include "ClientDefine.h"

#include "NetworkManager.h"

#include "Pawn.h"

#include "BaseModel.h"
#include "TransparentModel.h"
#include "StretchModel.h"

#include "GameFramework.h"

WGameFramework::WGameFramework(const std::string_view& inIPAddress)
	: m_hWnd()
	, tickCount()
	, networkManager(nullptr)
	, originPlayerModel(nullptr)
	, originOtherPlayerModel(nullptr)
	, originBackgroundModel(nullptr)
	, coverUIModel(nullptr)
	, broadcastAreaModel(nullptr)
	, playerCharacter(nullptr)
	, otherPlayerCont()
	, otherPlayerContLock()
	, backgroundActorCont()
	, coverUI()
	, broadcastAreaUI()
	, ipAddress(inIPAddress)
	, myClientKey()
{
	otherPlayerCont.clear();
}

WGameFramework::~WGameFramework()
{
	delete originPlayerModel;
	delete originOtherPlayerModel;
	delete originBackgroundModel;

	delete coverUIModel;
	delete broadcastAreaModel;

	otherPlayerCont.clear();

	/* auto */
	//playerCharacter.reset();
	//backgroundActor.reset();
}

//void WGameFramework::Reset()
//{
//}

//void WGameFramework::Clear()
//{
//}

void WGameFramework::Create(HWND hWnd)
{
	m_hWnd = hWnd;

	//build Object
	originPlayerModel = new TransparentModel(L"Resource/Image/Image_PlayerCharacter.png");
	originOtherPlayerModel = new TransparentModel(L"Resource/Image/Image_OtherCharacter.png");
	originBackgroundModel = new StretchModel(L"Resource/Image/Image_New_Background.png");
	coverUIModel = new StretchModel(L"Resource/Image/Image_Cover.png");
	broadcastAreaModel = new TransparentModel(L"Resource/Image/Image_BroadcastArea.png");
	//for( int i = 0; i < otherPlayerArr.size(); ++i)
	//	otherPlayerArr[i] = std::make_unique<Pawn>(originOtherPlayerModel,
	//		RenderData(0, 0, 100, 100, RGB(255, 0, 0)), false);
	
	const int tempBackgroundCount = 13;
	backgroundActorCont.reserve(tempBackgroundCount);
	
	for (int i = 0; i < tempBackgroundCount; ++i) 
	{
		backgroundActorCont.emplace_back();
		backgroundActorCont[i].reserve(tempBackgroundCount);
	}

	for (int i = 0; i < tempBackgroundCount; ++i)
	{
		for (int j = 0; j < tempBackgroundCount; ++j)
		{
			backgroundActorCont[i].emplace_back(std::make_unique<Pawn>(originBackgroundModel, RenderData(0, 0, 560, 560), i * 8, j * 8 ));
		}
	}

	coverUI = std::make_unique<BaseActor>(coverUIModel, RenderData(770, 0, 220, 800));
	broadcastAreaUI = std::make_unique<BaseActor>(broadcastAreaModel, RenderData(139, 139, 494, 494, COLOR::_WHITE));

	networkManager = std::make_unique<NetworkManager>(ipAddress, this);
}

void WGameFramework::OnDraw(HDC hdc)
{
	// ������ �켱������ �����ؾ��մϴ�!
	for (auto iter = backgroundActorCont.cbegin(); iter != backgroundActorCont.end(); ++iter)
	{
		for (auto inIter = (*iter).cbegin(); inIter != (*iter).cend(); ++inIter)
		{
			(*inIter)->Render(hdc);
		}
	}

	otherPlayerContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1
	for (auto iter = otherPlayerCont.cbegin(); iter != otherPlayerCont.cend(); ++iter) (iter->second)->Render(hdc);
	otherPlayerContLock.unlock(); //------------------------------------------------0

	if(playerCharacter != nullptr) playerCharacter->Render(hdc);

	coverUI->Render(hdc);

	broadcastAreaUI->Render(hdc);

}

void WGameFramework::OnUpdate(const float frameTime)
{
}

void WGameFramework::Mouse(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEV_MODE_
	switch (iMessage)
	{
		case WM_LBUTTONDOWN:
			std::cout << "MouseX : " << LOWORD(lParam) << " MouseY : " << HIWORD(lParam) << "\n";
		default:
			break;
	}
#endif
}

void WGameFramework::KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		case WM_KEYDOWN:
			switch (wParam)
			{
			case static_cast<WPARAM>(VK_KEY::VK_Q) :
				SendMessage(m_hWnd, WM_DESTROY, 0, 0);
				break;
			case static_cast<WPARAM>(VK_LEFT) :
			case static_cast<WPARAM>(VK_UP) :
			case static_cast<WPARAM>(VK_RIGHT) :
			case static_cast<WPARAM>(VK_DOWN) :
				networkManager->SendMoveData(static_cast<BYTE>(wParam) - VK_LEFT);
				break;
			}
			break;

		case WM_KEYUP:
			break;

		default:
			break;
	}
}

void WGameFramework::RecvLoginOK(const char* pBufferStart)
{
	//using namespace PACKET_DATA::SC;
	//LoginOk packet(pBufferStart[2]);
	myClientKey = static_cast<_ClientKeyType>(pBufferStart[2]);

#ifdef _DEV_MODE_
	std::cout << "[RECV] �α����� �����߽��ϴ�. ���� Ű���� : "<< myClientKey << "\n" ;
#endif
}

void WGameFramework::RecvPutPlayer(const char* pBufferStart)
{
	using namespace PACKET_DATA::SC;
	PutPlayer packet(pBufferStart[2], pBufferStart[3], pBufferStart[4]);

#ifdef _DEV_MODE_
	std::cout << "[RECV] ���ο� ĳ���͸� �����մϴ�. Ű���� : " << (int)packet.id << "��ġ x, y�� : " << (int)packet.x << " "<< (int)packet.y <<"\n";
#endif

	if (myClientKey == packet.id)
	{
		if (originPlayerModel == nullptr)
		{
			originPlayerModel = new TransparentModel(L"Resource/Image/Image_PlayerCharacter.png");
		}

#ifdef _DEV_MODE_
		std::cout << "[RECV] ��ĳ���Ͱ� �����˴ϴ� " << std::endl;
#endif

		playerCharacter = std::make_unique<Pawn>(originPlayerModel,
			RenderData(350, 350, 70, 70, COLOR::_RED), packet.x, packet.y);

		UpdateBackgroundActor();
	}
	else
	{
		otherPlayerContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1
		// �ȳ�! ���ο� �÷��̾�!
		otherPlayerCont.emplace_back(
			std::make_pair(packet.id, std::make_unique<Pawn>(originOtherPlayerModel,
				RenderData(0, 0, 70, 70, COLOR::_RED), packet.x, packet.y)
			)
		).second->UpdateRenderData(playerCharacter->GetPosition());

		otherPlayerContLock.unlock(); //------------------------------------------------0
	}
}

void WGameFramework::RecvRemovePlayer(const char* pBufferStart)
{
	using namespace PACKET_DATA::SC;
	RemovePlayer packet(pBufferStart[2]);

#ifdef _DEV_MODE_
	std::cout << "[RECV] ĳ���͸� �����մϴ�. Ű���� : " << packet.id << "\n";
#endif

	otherPlayerContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1
	// �߰��� �÷��̾�!!
	for (auto iter = otherPlayerCont.begin(); iter != otherPlayerCont.end(); ++iter)
	{
		if (iter->first == packet.id)
		{
			otherPlayerCont.erase(iter);
			break;
		}
	}
	otherPlayerContLock.unlock(); //------------------------------------------------0
}

void WGameFramework::RecvPosition(const char* pBufferStart)
{
	using namespace PACKET_DATA::SC;
	Position packet(pBufferStart[2], pBufferStart[3], pBufferStart[4]);

#ifdef _DEV_MODE_
	std::cout << "[RECV] ĳ���Ͱ� �̵��մϴ�. Ű���� : " << packet.id << "��ġ x, y�� : " << packet.x << " " << packet.y << "\n";
#endif

	if (myClientKey == packet.id)
	{
#ifdef _DEV_MODE_
		std::cout << "�� ĳ���Ͱ� �̵��մϴ�. ��ġ x, y�� : " << packet.x << " " << packet.y << "\n";
#endif
		playerCharacter->SetOnlyActorPositionNotUpdateRenderData(std::make_pair(packet.x, packet.y));
		UpdateOtherClient();
		UpdateBackgroundActor();
	}
	else
	{
		for (auto iter = otherPlayerCont.begin(); iter != otherPlayerCont.end(); ++iter)
		{
			if (iter->first == packet.id)
			{
				(iter->second)->SetPosition(std::make_pair(packet.x, packet.y), playerCharacter->GetPosition());
				break;
			}
		}
	}
}

void WGameFramework::UpdateOtherClient()
{
	for (auto iter = otherPlayerCont.begin(); iter != otherPlayerCont.end(); ++iter)
	{
		(iter->second)->UpdateRenderData(playerCharacter->GetPosition());
	}
}

void WGameFramework::UpdateBackgroundActor()
{
#ifdef _DEV_MODE_
	std::cout << "backgroundActorCont�� ��ġ�� �����մϴ�." << "\n";
#endif
	for (auto iter = backgroundActorCont.begin(); iter != backgroundActorCont.end(); ++iter)
	{
		for (auto inIter = (*iter).cbegin(); inIter != (*iter).cend(); ++inIter)
		{
			(*inIter)->UpdateRenderData(playerCharacter->GetPosition());
		}
	}
}

/*
void WGameFramework::SetCharactersLocation(const std::pair<std::array<std::pair<UINT8, UINT8>, GLOBAL_DEFINE::MAX_CLIENT + 1>, int> inCont, const bool inMyCharacterInclude)
{
	auto[pRecvCharacterPositionArr, connectedPlayerCount] = inCont;

	if(inMyCharacterInclude)
		playerCharacter->SetPosition(pRecvCharacterPositionArr[0]);

	for (int i = 0; i < connectedPlayerCount - 1; ++i)
	{
		otherPlayerArr[i]->SetPosition(pRecvCharacterPositionArr[i + 1]);
		otherPlayerArr[i]->SetRender(true);
	}
	for (int i = connectedPlayerCount - 1; i < GLOBAL_DEFINE::MAX_CLIENT + 1; ++i) otherPlayerArr[i]->SetRender(false);
}
*/