/*
	kpu-warp-winapi-framework		Copyright ⓒ https://github.com/KPU-WARP

	#0. 해당 프로젝트는 한국산업기술대 게임공학과 학술 소모임 WARP의 오픈소스 WinAPI Framework입니다.
	#1. 관련 세부 정보는 깃헙 저장소 리드미에서 확인하실 수 있습니다.
		- https://github.com/KPU-WARP/winapi-framework

	#2. 기타 문의는, KoreaGameMaker@gmail.com으로 부탁드립니다. 감사합니다 :)
*/

#include "stdafx.h"
#include "ClientDefine.h"

#include "Pawn.h"

#include "NetworkManager.h"
#include "BaseModel.h"
#include "RenderModelManager.h"

#include "GameFramework.h"

WGameFramework::WGameFramework(const std::string_view& inIPAddress)
	: m_hWnd()
	, networkManager(nullptr)
	, renderModelManager(nullptr)
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
	otherPlayerCont.clear();
	monsterCont.clear();
}

void WGameFramework::Create(HWND hWnd)
{
	m_hWnd = hWnd;

	renderModelManager = std::make_unique<RenderModelManager>();

	const int tempBackgroundCount = 10;
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
			switch (i % 5)
			{
			case 0:
				backgroundActorCont[i].emplace_back(
					std::make_unique<Pawn>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::BACKGROUND_0)
						, RenderData(0, 0, 1200, 1200), i * 30, j * 30));
				break;
			case 1:
				backgroundActorCont[i].emplace_back(
					std::make_unique<Pawn>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::BACKGROUND_1)
						, RenderData(0, 0, 1200, 1200), i * 30, j * 30));
				break;
			case 2:
				backgroundActorCont[i].emplace_back(
					std::make_unique<Pawn>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::BACKGROUND_2)
						, RenderData(0, 0, 1200, 1200), i * 30, j * 30));
				break;
			case 3:
				backgroundActorCont[i].emplace_back(
					std::make_unique<Pawn>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::BACKGROUND_3)
						, RenderData(0, 0, 1200, 1200), i * 30, j * 30));
				break;
			case 4:
				backgroundActorCont[i].emplace_back(
					std::make_unique<Pawn>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::BACKGROUND_4)
						, RenderData(0, 0, 1200, 1200), i * 30, j * 30));
				break;
			}
		}
	}

	coverUI = std::make_unique<BaseActor>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::COVER_UI), RenderData(800, 0, 200, 800));
	broadcastAreaUI = std::make_unique<BaseActor>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::BROADCAST_UI), RenderData(0, 0, 800, 800, COLOR::_WHITE));
	
	networkManager = std::make_unique<NetworkManager>(ipAddress, this);
}

void WGameFramework::OnDraw(HDC hdc)
{
	// 렌더링 우선순위를 주의해야합니다!
	for (auto iter = backgroundActorCont.begin(); iter != backgroundActorCont.end(); ++iter)
	{
		for (auto inIter = (*iter).begin(); inIter != (*iter).end(); ++inIter)
		{
			(*inIter)->Render(hdc);
		}
	}

	monsterContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1
	for (auto iter = monsterCont.cbegin(); iter != monsterCont.cend(); ++iter) (iter->second)->Render(hdc);
	monsterContLock.unlock(); //------------------------------------------------0

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
//#ifdef _DEV_MODE_
	switch (iMessage)
	{
		case WM_LBUTTONDOWN:
			std::cout << "MouseX : " << LOWORD(lParam) << " MouseY : " << HIWORD(lParam) << "\n";
		default:
			break;
	}
//#endif
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

void WGameFramework::RecvLoginOK(char* pBufferStart)
{
	using namespace PACKET_DATA::MAIN_TO_CLIENT;
	LoginOk* packet = reinterpret_cast<LoginOk *>(pBufferStart);
	myClientKey = packet->key;

#ifdef _DEV_MODE_
	std::cout << "[RECV] 로그인에 성공했습니다. 나의 키값은 : "<< (int)myClientKey << "\n" ;
#endif
	//if (myClientKey == realKey)
	//{
	if (packet->job == JOB_TYPE::ARCHER)
		playerCharacter = std::make_unique<Pawn>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::PLAYER_ARCHER),
			RenderData(350, 350, GLOBAL_DEFINE::BLOCK_WIDTH_SIZE, GLOBAL_DEFINE::BLOCK_HEIGHT_SIZE, COLOR::_RED), packet->x, packet->y);
	else if (packet->job == JOB_TYPE::KNIGHT)
		playerCharacter = std::make_unique<Pawn>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::PLAYER_KNIGHT),
			RenderData(350, 350, GLOBAL_DEFINE::BLOCK_WIDTH_SIZE, GLOBAL_DEFINE::BLOCK_HEIGHT_SIZE, COLOR::_RED), packet->x, packet->y);
	else if (packet->job == JOB_TYPE::WITCH)
		playerCharacter = std::make_unique<Pawn>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::PLAYER_WITCH),
			RenderData(350, 350, GLOBAL_DEFINE::BLOCK_WIDTH_SIZE, GLOBAL_DEFINE::BLOCK_HEIGHT_SIZE, COLOR::_RED), packet->x, packet->y);
	else
		std::cout << "뭐여! 나님은 무슨 직업이십니까?" << std::endl;

	UpdateBackgroundActor();
	//}
}

void WGameFramework::RecvPutPlayer(char* pBufferStart)
{
	using namespace PACKET_DATA::MAIN_TO_CLIENT;
	PutPlayer* packet = reinterpret_cast<PutPlayer *>(pBufferStart);

#ifdef _DEV_MODE_
	std::cout << "[RECV] 새로운 캐릭터를 생성합니다. 키값은 : " << (int)packet->id << "위치 x, y는 : " << (int)packet->x << " "<< (int)packet->y <<"\n";
#endif
	using namespace BIT_CONVERTER;
	switch (auto[objectType, realKey] = BIT_CONVERTER::WhatIsYourTypeAndRealKey(packet->key); objectType)
	{
	case OBJECT_TYPE::PLAYER:
		{
		RENDER_MODEL_TYPE tempRenderModelType{};
		if (packet->job == JOB_TYPE::ARCHER)	tempRenderModelType = RENDER_MODEL_TYPE::OTHER_PLAYER_ARCHER;
		else if (packet->job == JOB_TYPE::KNIGHT)	tempRenderModelType = RENDER_MODEL_TYPE::OTHER_PLAYER_KNIGHT;
		else if (packet->job == JOB_TYPE::WITCH)	tempRenderModelType = RENDER_MODEL_TYPE::OTHER_PLAYER_WITCH;
		else std::cout << "마 그럼 니는 누군디! " << std::endl;

		otherPlayerContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1
		// 안녕! 새로운 플레이어!

		std::cout << "[PUT] 플레이어 (" << packet->key << ") 등장!" << std::endl;

		otherPlayerCont.emplace_back(
			std::make_pair(realKey, std::make_unique<Pawn>(renderModelManager->GetRenderModel(tempRenderModelType),
				RenderData(0, 0, GLOBAL_DEFINE::BLOCK_WIDTH_SIZE, GLOBAL_DEFINE::BLOCK_HEIGHT_SIZE, COLOR::_RED), packet->x, packet->y)
			)
		).second->UpdateRenderData(playerCharacter->GetPosition());

		otherPlayerContLock.unlock(); //------------------------------------------------0
		}
		break;
	case OBJECT_TYPE::MONSTER:
		{
		RENDER_MODEL_TYPE tempRenderModelType{};
		if (packet->job == JOB_TYPE::SLIME)	tempRenderModelType = RENDER_MODEL_TYPE::MONSTER_SLIME;
		else if (packet->job == JOB_TYPE::GOLEM)	tempRenderModelType = RENDER_MODEL_TYPE::MONSTER_GOLME;
		else if (packet->job == JOB_TYPE::DRAGON)	tempRenderModelType = RENDER_MODEL_TYPE::MONSTER_DRAGON;
		else std::cout << "마 그럼 니는 누군디! " << std::endl;

		monsterContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1

		monsterCont.emplace_back(
			std::make_pair(realKey, std::make_unique<Pawn>(renderModelManager->GetRenderModel(tempRenderModelType),
				RenderData(0, 0, GLOBAL_DEFINE::BLOCK_WIDTH_SIZE, GLOBAL_DEFINE::BLOCK_HEIGHT_SIZE, COLOR::_RED), packet->x, packet->y)
			)
		).second->UpdateRenderData(playerCharacter->GetPosition());

		monsterContLock.unlock(); //------------------------------------------------0
		}
		break;
	case OBJECT_TYPE::NPC:
		std::cout << "야 NPC 없는데 무슨 NPC가 오냐 키값 뭔데임마 " << (int)packet->key << " // "<< std::bitset<32>(packet->key) << " // " << realKey << std::endl;
		break;
	default:
		std::cout << "마! 니는 뭐냐 이자식이 " << (int)packet->key << " // " << std::bitset<32>(packet->key) << " // " << realKey << std::endl;
		break;
	}
}

void WGameFramework::RecvRemovePlayer(char* pBufferStart)
{
	using namespace PACKET_DATA::MAIN_TO_CLIENT;
	RemovePlayer* packet = reinterpret_cast<RemovePlayer *>(pBufferStart);
	using namespace BIT_CONVERTER;
#ifdef _DEV_MODE_
	std::cout << "[RECV] 캐릭터를 제거합니다. 키값은 : " << (int)(packet->id) << "\n";
#endif
	switch (auto[objectType, realKey] = BIT_CONVERTER::WhatIsYourTypeAndRealKey(packet->key); objectType)
	{
	case OBJECT_TYPE::PLAYER:
		std::cout << "[Remove] 플레이어 (" << packet->key << ") 안녕!" << std::endl;

		otherPlayerContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1
		// 잘가랏 플레이어!!
		for (auto iter = otherPlayerCont.begin(); iter != otherPlayerCont.end(); ++iter)
		{
			if (iter->first == realKey)
			{
				otherPlayerCont.erase(iter);
				break;
			}
		}
		otherPlayerContLock.unlock(); //------------------------------------------------0
		break;
	case OBJECT_TYPE::MONSTER:
		monsterContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1
		for (auto iter = monsterCont.begin(); iter != monsterCont.end(); ++iter)
		{
			if (iter->first == realKey)
			{
				monsterCont.erase(iter);
				break;
			}
		}
		monsterContLock.unlock(); //------------------------------------------------0
		break;
	case OBJECT_TYPE::NPC:
		std::cout << "야 NPC 없는데 무슨 NPC가 오냐" << std::endl;
		break;
	default:
		std::cout << "마! 니는 뭐냐" << std::endl;
		break;
	}
}

void WGameFramework::RecvPosition(char* pBufferStart)
{
	using namespace PACKET_DATA::MAIN_TO_CLIENT;
	using namespace BIT_CONVERTER;

	Position* packet = reinterpret_cast<Position *>(pBufferStart);

#ifdef _DEV_MODE_
	std::cout << "[RECV] 캐릭터가 이동합니다. 키값은 : " << (unsigned int)packet->id << "위치 x, y는 : " << (int)packet->x << " " << (int)packet->y << "\n";
#endif

	switch (auto[objectType, realKey] = BIT_CONVERTER::WhatIsYourTypeAndRealKey(packet->key); objectType)
	{
	case OBJECT_TYPE::PLAYER:
		if (myClientKey == realKey)
		{
#ifdef _DEV_MODE_
			//std::cout << "내 캐릭터가 이동합니다. 위치 x, y는 : " << packet.x << " " << packet.y << "\n";
#endif
			playerCharacter->SetOnlyActorPositionNotUpdateRenderData(std::make_pair(packet->x, packet->y));
			UpdateOtherObject();
			UpdateBackgroundActor();
		}
		else
		{
			for (auto iter = otherPlayerCont.begin(); iter != otherPlayerCont.end(); ++iter)
			{
				if (iter->first == realKey)
				{
					(iter->second)->SetPosition(std::make_pair(packet->x, packet->y), playerCharacter->GetPosition());
					break;
				}
			}
		}
		break;
	case OBJECT_TYPE::MONSTER:
		monsterContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1
		for (auto iter = monsterCont.begin(); iter != monsterCont.end(); ++iter)
		{
			if (iter->first == realKey)
			{
				(iter->second)->SetPosition(std::make_pair(packet->x, packet->y), playerCharacter->GetPosition());
				break;
			}
		}
		monsterContLock.unlock(); //------------------------------------------------0
		break;
	default:
		std::cout << "마! 니는 뭐냐" << std::endl;
		break;
	}
}

void WGameFramework::RecvChat(char* pBufferStart)
{
	//using namespace PACKET_DATA::;
	//Chat packet(pBufferStart);
}

void WGameFramework::RecvLoginFail(char* pBufferStart)
{
	using namespace PACKET_DATA::MAIN_TO_CLIENT;
	using namespace BIT_CONVERTER;

	LoginFail* packet = reinterpret_cast<LoginFail *>(pBufferStart);
	switch (packet->failReason)
	{
		// 0이면 없는 계정, 1이면 이미 로그인한 계정, 2이면 만들려고 했는데 이미 있는 계정
	case 0:
		std::cout << "해당 닉네임을 가지고 있는 계정이 없습니다. 확인 후 다시 시도하세요. \n";
		break;
	case 1:
		std::cout << "해당 계정은 이미 로그인한 계정입니다. \n";
		break;
	case 2:
		std::cout << "해당 닉네임의 계정이 이미 존재합니다. 다른 닉네임으로 회원가입하세요. \n";
		break;
	}

	networkManager->LogInOrSignUpProcess();
}

void WGameFramework::UpdateOtherObject()
{
	for (auto iter = otherPlayerCont.begin(); iter != otherPlayerCont.end(); ++iter)
	{
		(iter->second)->UpdateRenderData(playerCharacter->GetPosition());
	}
	for (auto iter = monsterCont.begin(); iter != monsterCont.end(); ++iter)
	{
		(iter->second)->UpdateRenderData(playerCharacter->GetPosition());
	}
}

void WGameFramework::UpdateBackgroundActor()
{
#ifdef _DEV_MODE_
	//std::cout << "backgroundActorCont의 위치를 갱신합니다." << "\n";
#endif
	for (auto iter = backgroundActorCont.begin(); iter != backgroundActorCont.end(); ++iter)
	{
		for (auto inIter = (*iter).begin(); inIter != (*iter).end(); ++inIter)
		{
			(*inIter)->UpdateRenderData(playerCharacter->GetPosition());
		}
	}
}
