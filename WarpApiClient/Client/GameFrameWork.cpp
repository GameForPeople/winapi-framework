/*
	kpu-warp-winapi-framework		Copyright �� https://github.com/KPU-WARP

	#0. �ش� ������Ʈ�� �ѱ��������� ���Ӱ��а� �м� �Ҹ��� WARP�� ���¼ҽ� WinAPI Framework�Դϴ�.
	#1. ���� ���� ������ ���� ����� ����̿��� Ȯ���Ͻ� �� �ֽ��ϴ�.
		- https://github.com/KPU-WARP/winapi-framework

	#2. ��Ÿ ���Ǵ�, KoreaGameMaker@gmail.com���� ��Ź�帳�ϴ�. �����մϴ� :)
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

	const int tempBackgroundCount = 40;
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
			backgroundActorCont[i].emplace_back(
				std::make_unique<Pawn>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::BACKGROUND)
					, RenderData(0, 0, 800, 800), i * 20, j * 20));
		}
	}

	coverUI = std::make_unique<BaseActor>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::COVER_UI), RenderData(800, 0, 200, 800));
	broadcastAreaUI = std::make_unique<BaseActor>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::BROADCAST_UI), RenderData(70, 70, 600, 600, COLOR::_WHITE));
	
	networkManager = std::make_unique<NetworkManager>(ipAddress, this);
}

void WGameFramework::OnDraw(HDC hdc)
{
	// ������ �켱������ �����ؾ��մϴ�!
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
	myClientKey = packet->id;

#ifdef _DEV_MODE_
	std::cout << "[RECV] �α��ο� �����߽��ϴ�. ���� Ű���� : "<< (int)myClientKey << "\n" ;
#endif
}

void WGameFramework::RecvPutPlayer(char* pBufferStart)
{
	using namespace PACKET_DATA::MAIN_TO_CLIENT;
	PutPlayer* packet = reinterpret_cast<PutPlayer *>(pBufferStart);

#ifdef _DEV_MODE_
	std::cout << "[RECV] ���ο� ĳ���͸� �����մϴ�. Ű���� : " << (int)packet->id << "��ġ x, y�� : " << (int)packet->x << " "<< (int)packet->y <<"\n";
#endif
	using namespace BIT_CONVERTER;
	switch (auto[objectType, realKey] = BIT_CONVERTER::WhatIsYourTypeAndRealKey(packet->id); objectType)
	{
	case OBJECT_TYPE::PLAYER:
		if (myClientKey == realKey)
		{
			playerCharacter = std::make_unique<Pawn>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::PLAYER),
				RenderData(350, 350, GLOBAL_DEFINE::BLOCK_WIDTH_SIZE, GLOBAL_DEFINE::BLOCK_HEIGHT_SIZE, COLOR::_RED), packet->x, packet->y);

			UpdateBackgroundActor();
		}
		else
		{
			otherPlayerContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1
			// �ȳ�! ���ο� �÷��̾�!
			otherPlayerCont.emplace_back(
				std::make_pair(realKey, std::make_unique<Pawn>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::OTHER_PLAYER),
					RenderData(0, 0, GLOBAL_DEFINE::BLOCK_WIDTH_SIZE, GLOBAL_DEFINE::BLOCK_HEIGHT_SIZE, COLOR::_RED), packet->x, packet->y)
				)
			).second->UpdateRenderData(playerCharacter->GetPosition());

			otherPlayerContLock.unlock(); //------------------------------------------------0
		}
		break;
	case OBJECT_TYPE::MONSTER:
		monsterContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1

		monsterCont.emplace_back(
			std::make_pair(realKey, std::make_unique<Pawn>(renderModelManager->GetRenderModel(RENDER_MODEL_TYPE::MONSTER),
				RenderData(0, 0, GLOBAL_DEFINE::BLOCK_WIDTH_SIZE, GLOBAL_DEFINE::BLOCK_HEIGHT_SIZE, COLOR::_RED), packet->x, packet->y)
			)
		).second->UpdateRenderData(playerCharacter->GetPosition());

		monsterContLock.unlock(); //------------------------------------------------0
		break;
	case OBJECT_TYPE::NPC:
		std::cout << "�� NPC ���µ� ���� NPC�� ���� Ű�� �����Ӹ� " << (int)packet->id << " // "<< std::bitset<32>(packet->id) << " // " << realKey << std::endl;
		break;
	default:
		std::cout << "��! �ϴ� ���� ���ڽ��� " << (int)packet->id << " // " << std::bitset<32>(packet->id) << " // " << realKey << std::endl;
		break;
	}
}

void WGameFramework::RecvRemovePlayer(char* pBufferStart)
{
	using namespace PACKET_DATA::MAIN_TO_CLIENT;
	RemovePlayer* packet = reinterpret_cast<RemovePlayer *>(pBufferStart);
	using namespace BIT_CONVERTER;
#ifdef _DEV_MODE_
	std::cout << "[RECV] ĳ���͸� �����մϴ�. Ű���� : " << (int)(packet->id) << "\n";
#endif
	switch (auto[objectType, realKey] = BIT_CONVERTER::WhatIsYourTypeAndRealKey(packet->id); objectType)
	{
	case OBJECT_TYPE::PLAYER:
		otherPlayerContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1
		// �߰��� �÷��̾�!!
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
		std::cout << "�� NPC ���µ� ���� NPC�� ����" << std::endl;
		break;
	default:
		std::cout << "��! �ϴ� ����" << std::endl;
		break;
	}
}

void WGameFramework::RecvPosition(char* pBufferStart)
{
	using namespace PACKET_DATA::MAIN_TO_CLIENT;
	using namespace BIT_CONVERTER;

	Position* packet = reinterpret_cast<Position *>(pBufferStart);

#ifdef _DEV_MODE_
	std::cout << "[RECV] ĳ���Ͱ� �̵��մϴ�. Ű���� : " << (unsigned int)packet->id << "��ġ x, y�� : " << (int)packet->x << " " << (int)packet->y << "\n";
#endif

	switch (auto[objectType, realKey] = BIT_CONVERTER::WhatIsYourTypeAndRealKey(packet->id); objectType)
	{
	case OBJECT_TYPE::PLAYER:
		if (myClientKey == realKey)
		{
#ifdef _DEV_MODE_
			//std::cout << "�� ĳ���Ͱ� �̵��մϴ�. ��ġ x, y�� : " << packet.x << " " << packet.y << "\n";
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
		std::cout << "��! �ϴ� ����" << std::endl;
		break;
	}
}

void WGameFramework::RecvChat(char* pBufferStart)
{
	//using namespace PACKET_DATA::;
	//Chat packet(pBufferStart);
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
	//std::cout << "backgroundActorCont�� ��ġ�� �����մϴ�." << "\n";
#endif
	for (auto iter = backgroundActorCont.begin(); iter != backgroundActorCont.end(); ++iter)
	{
		for (auto inIter = (*iter).begin(); inIter != (*iter).end(); ++inIter)
		{
			(*inIter)->UpdateRenderData(playerCharacter->GetPosition());
		}
	}
}
