/*
	kpu-warp-winapi-framework		Copyright ⓒ https://github.com/KPU-WARP

	#0. 해당 프로젝트는 한국산업기술대 게임공학과 학술 소모임 WARP의 오픈소스 WinAPI Framework입니다.
	#1. 관련 세부 정보는 깃헙 저장소 리드미에서 확인하실 수 있습니다.
		- https://github.com/KPU-WARP/winapi-framework

	#2. 기타 문의는, KoreaGameMaker@gmail.com으로 부탁드립니다. 감사합니다 :)
*/

#include "stdafx.h"

#include "ClientDefine.h"

#include "NetworkManager.h"

#include "Pawn.h"
#include "TransparentModel.h"
#include "StretchModel.h"

#include "GameFramework.h"

WGameFramework::WGameFramework(const std::string_view& inIPAddress)
	: networkManager()
	, tickCount()
	, backgroundModel()
	, m_hWnd()
	, myClientKey()
	, originPlayerModel()
	, originOtherPlayerModel()
	, otherPlayerContLock()
{
	networkManager = std::make_unique<NetworkManager>(inIPAddress, this);
	otherPlayerCont.clear();
}

WGameFramework::~WGameFramework()
{
	delete originPlayerModel;
	delete backgroundModel;
	delete originOtherPlayerModel;

	otherPlayerCont.clear();
	/* auto */
	//playerCharacter.reset();
	//backgroundActor.reset();
}

void WGameFramework::Reset()
{
}

void WGameFramework::Clear()
{
}

void WGameFramework::Create(HWND hWnd)
{
	m_hWnd = hWnd;

	//build Object
	originPlayerModel = new TransparentModel(L"Resource/Image/Image_PlayerCharacter.png");
	originOtherPlayerModel = new TransparentModel(L"Resource/Image/Image_OtherCharacter.png");
	backgroundModel = new StretchModel(L"Resource/Image/Image_Background.png");
	
	//for( int i = 0; i < otherPlayerArr.size(); ++i)
	//	otherPlayerArr[i] = std::make_unique<Pawn>(originOtherPlayerModel,
	//		RenderData(0, 0, 100, 100, RGB(255, 0, 0)), false);
	
	backgroundActor = std::make_unique<BaseActor>(backgroundModel, 
		RenderData(0,0, 1000, 770));
}

void WGameFramework::OnDraw(HDC hdc)
{
	// 렌더링 우선순위를 주의해야합니다!
	backgroundActor->Render(hdc);

	otherPlayerContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1
	for (auto iter = otherPlayerCont.cbegin(); iter != otherPlayerCont.cend(); ++iter) (iter->second)->Render(hdc);
	otherPlayerContLock.unlock(); //------------------------------------------------0

	if(playerCharacter != nullptr) playerCharacter->Render(hdc);
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
}

void WGameFramework::RecvPutPlayer(const char* pBufferStart)
{
	using namespace PACKET_DATA::SC;
	PutPlayer packet(pBufferStart[2], pBufferStart[3], pBufferStart[4]);

	if (myClientKey == packet.id)
	{
		playerCharacter = std::make_unique<Pawn>(originPlayerModel,
			RenderData(0, 0, 100, 100, COLOR::_RED), packet.x, packet.y);

		return;
	}

	otherPlayerContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1
	// 안녕! 새로운 플레이어!
	otherPlayerCont.emplace_back(
		std::make_pair( packet.id, std::make_unique<Pawn>(originOtherPlayerModel,
			RenderData(0, 0, 100, 100, COLOR::_RED), packet.x , packet.y	)
		)
	);
	otherPlayerContLock.unlock(); //------------------------------------------------0
}

void WGameFramework::RecvRemovePlayer(const char* pBufferStart)
{
	using namespace PACKET_DATA::SC;
	RemovePlayer packet(pBufferStart[2]);

	otherPlayerContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1
	// 잘가랏 플레이어!!
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

	// 너의 새 위치는?
	for (auto iter = otherPlayerCont.begin(); iter != otherPlayerCont.end(); ++iter)
	{
		if (iter->first == packet.id)
		{
			(iter->second)->SetPosition(std::make_pair(packet.x, packet.y));
			break;
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