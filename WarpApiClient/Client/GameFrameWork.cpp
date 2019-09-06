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
	, isLogin(false)
	, isDeath(false)
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
	
	notLoginUI = std::make_unique<CImage>();
	notLoginUI->Load(L"Resource/Image/Image_NotLogin.png");
	
	diedUI = std::make_unique<CImage>();
	diedUI->Load(L"Resource/Image/Image_Died.png");

	numberUICont.resize(10);
	numberUICont[0].Load(L"Resource/Image/Combo_Orange_Yellow_0.png");
	numberUICont[1].Load(L"Resource/Image/Combo_Orange_Yellow_1.png");
	numberUICont[2].Load(L"Resource/Image/Combo_Orange_Yellow_2.png");
	numberUICont[3].Load(L"Resource/Image/Combo_Orange_Yellow_3.png");
	numberUICont[4].Load(L"Resource/Image/Combo_Orange_Yellow_4.png");
	numberUICont[5].Load(L"Resource/Image/Combo_Orange_Yellow_5.png");
	numberUICont[6].Load(L"Resource/Image/Combo_Orange_Yellow_6.png");
	numberUICont[7].Load(L"Resource/Image/Combo_Orange_Yellow_7.png");
	numberUICont[8].Load(L"Resource/Image/Combo_Orange_Yellow_8.png");
	numberUICont[9].Load(L"Resource/Image/Combo_Orange_Yellow_9.png");

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

	if (level)
	{
		const int sizeBuffer = 35;
		if(level > 9) numberUICont[level / 10].TransparentBlt(hdc, 900, 400, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		numberUICont[level % 10].TransparentBlt(hdc, 940, 400, sizeBuffer, sizeBuffer, COLOR::_WHITE);
	}

	if (const int sizeBuffer = 20; myExp)
	{
		int tempExp = myExp % 1000;

		if (tempExp > 99) numberUICont[tempExp / 100].TransparentBlt(hdc, 900, 460, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		if (tempExp > 9) numberUICont[(tempExp / 10) % 10].TransparentBlt(hdc, 930, 460, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		numberUICont[tempExp % 10].TransparentBlt(hdc, 960, 460, sizeBuffer, sizeBuffer, COLOR::_WHITE);
	}
	else numberUICont[0].TransparentBlt(hdc, 960, 460, sizeBuffer, sizeBuffer, COLOR::_WHITE);

	if (const int sizeBuffer = 20; hp)
	{
		if (hp > 99) numberUICont[hp / 100].TransparentBlt(hdc, 900, 510, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		if (hp > 9) numberUICont[(hp / 10) % 10].TransparentBlt(hdc, 930, 510, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		numberUICont[hp % 10].TransparentBlt(hdc, 960, 510, sizeBuffer, sizeBuffer, COLOR::_WHITE);
	}
	else numberUICont[0].TransparentBlt(hdc, 960, 510, sizeBuffer, sizeBuffer, COLOR::_WHITE);

	if (const int sizeBuffer = 20; mp)
	{
		if (mp > 99) numberUICont[mp / 100].TransparentBlt(hdc, 900, 560, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		if (mp > 9) numberUICont[(mp / 10) % 10].TransparentBlt(hdc, 930, 560, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		numberUICont[mp % 10].TransparentBlt(hdc, 960, 560, sizeBuffer, sizeBuffer, COLOR::_WHITE);
	}
	else numberUICont[0].TransparentBlt(hdc, 960, 560, sizeBuffer, sizeBuffer, COLOR::_WHITE);

	if (const int sizeBuffer = 20; redCount)
	{
		if (redCount > 99) numberUICont[redCount / 100].TransparentBlt(hdc, 900, 610, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		if (redCount > 9) numberUICont[(redCount / 10) % 10].TransparentBlt(hdc, 930, 610, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		numberUICont[redCount % 10].TransparentBlt(hdc, 960, 610, sizeBuffer, sizeBuffer, COLOR::_WHITE);
	}
	else numberUICont[0].TransparentBlt(hdc, 960, 610, sizeBuffer, sizeBuffer, COLOR::_WHITE);

	if (const int sizeBuffer = 20; blueCount)
	{
		if (blueCount > 99) numberUICont[blueCount / 100].TransparentBlt(hdc, 900, 660, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		if (blueCount > 9) numberUICont[(blueCount / 10) % 10].TransparentBlt(hdc, 930, 660, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		numberUICont[blueCount % 10].TransparentBlt(hdc, 960, 660, sizeBuffer, sizeBuffer, COLOR::_WHITE);
	}
	else numberUICont[0].TransparentBlt(hdc, 960, 660, sizeBuffer, sizeBuffer, COLOR::_WHITE);

	if (const int sizeBuffer = 15; money)
	{
		if (money > 999) numberUICont[(money / 1000) % 10].TransparentBlt(hdc, 900, 710, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		if (money > 99) numberUICont[(money / 100) % 10].TransparentBlt(hdc, 920, 710, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		if (money > 9) numberUICont[(money / 10) % 10].TransparentBlt(hdc, 940, 710, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		numberUICont[money % 10].TransparentBlt(hdc, 960, 710, sizeBuffer, sizeBuffer, COLOR::_WHITE);
	}
	else numberUICont[0].TransparentBlt(hdc, 960, 710, sizeBuffer, sizeBuffer, COLOR::_WHITE);

	if (const int sizeBuffer = 15; posX)
	{
		if (posX > 99) numberUICont[(posX / 100) % 10].TransparentBlt(hdc, 10, 30, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		if (posX > 9) numberUICont[(posX / 10) % 10].TransparentBlt(hdc, 25, 30, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		numberUICont[posX % 10].TransparentBlt(hdc, 40, 30, sizeBuffer, sizeBuffer, COLOR::_WHITE);
	}
	else numberUICont[0].TransparentBlt(hdc, 40, 30, sizeBuffer, sizeBuffer, COLOR::_WHITE);

	if (const int sizeBuffer = 15; posY)
	{
		if (posY > 99) numberUICont[(posY / 100) % 10].TransparentBlt(hdc, 10, 50, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		if (posY > 9) numberUICont[(posY / 10) % 10].TransparentBlt(hdc, 25, 50, sizeBuffer, sizeBuffer, COLOR::_WHITE);
		numberUICont[posY % 10].TransparentBlt(hdc, 40, 50, sizeBuffer, sizeBuffer, COLOR::_WHITE);
	}
	else numberUICont[0].TransparentBlt(hdc, 40, 50, sizeBuffer, sizeBuffer, COLOR::_WHITE);

	if(!isLogin) notLoginUI->AlphaBlend(hdc, 0, 0, 1000, 835, 0, 0, 1000, 835, 200, 000);
	if(isDeath) diedUI->AlphaBlend(hdc, 0, 0, 1000, 835, 0, 0, 1000, 835, 200, 000);
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
	if (isLogin) {
		switch (iMessage)
		{
		case WM_KEYDOWN:
			switch (wParam)
			{
				case static_cast<WPARAM>(VK_KEY::VK_P) :
				{
					SendMessage(m_hWnd, WM_DESTROY, 0, 0);
					break;
				}
				case static_cast<WPARAM>(VK_KEY::VK_A) :
				{
					networkManager->SendAttack(0);
					break;
				}
				case static_cast<WPARAM>(VK_KEY::VK_S) :
				{
					networkManager->SendAttack(1);
					break;
				}
				case static_cast<WPARAM>(VK_KEY::VK_D) :
				{
					networkManager->SendAttack(2);
					break;
				}
				case static_cast<WPARAM>(VK_KEY::VK_Q) :
				{
					networkManager->SendItem(0);	//RED P
					break;
				}
				case static_cast<WPARAM>(VK_KEY::VK_W) :
				{
					networkManager->SendItem(1);	//Blue P
					break;
				}
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
}

void WGameFramework::RecvLoginOK(char* pBufferStart)
{
	using namespace PACKET_DATA::MAIN_TO_CLIENT;
	LoginOk* packet = reinterpret_cast<LoginOk *>(pBufferStart);
	myClientKey = packet->key;

#ifdef _DEV_MODE_
	std::cout << "[RECV] �α��ο� �����߽��ϴ�. ���� Ű���� : "<< (int)myClientKey << "\n" ;
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
		std::cout << "����! ������ ���� �����̽ʴϱ�?" << std::endl;

	level = packet->level;
	hp = packet->hp;
	mp = packet->mp;
	job = packet->job;
	myExp = packet->exp;

	redCount = packet->redCount;
	blueCount = packet->blueCount;
	treeCount = packet->treeCount;

	money = packet->money;

	UpdateBackgroundActor();

	isLogin = true;
}

void WGameFramework::RecvPutPlayer(char* pBufferStart)
{
	using namespace PACKET_DATA::MAIN_TO_CLIENT;
	PutPlayer* packet = reinterpret_cast<PutPlayer *>(pBufferStart);

	using namespace BIT_CONVERTER;
	if (auto[objectType, realKey] = BIT_CONVERTER::WhatIsYourTypeAndRealKey(packet->key); objectType == OBJECT_TYPE::PLAYER)
	{
		if (myClientKey == realKey)
		{
			isDeath = false;
		}
	}

#ifdef _DEV_MODE_
	std::cout << "[RECV] ���ο� ĳ���͸� �����մϴ�. Ű���� : " << (int)packet->id << "��ġ x, y�� : " << (int)packet->x << " "<< (int)packet->y <<"\n";
#endif
	switch (auto[objectType, realKey] = BIT_CONVERTER::WhatIsYourTypeAndRealKey(packet->key); objectType)
	{
	case OBJECT_TYPE::PLAYER:
		{
		RENDER_MODEL_TYPE tempRenderModelType{};
		if (packet->job == JOB_TYPE::ARCHER)	tempRenderModelType = RENDER_MODEL_TYPE::OTHER_PLAYER_ARCHER;
		else if (packet->job == JOB_TYPE::KNIGHT)	tempRenderModelType = RENDER_MODEL_TYPE::OTHER_PLAYER_KNIGHT;
		else if (packet->job == JOB_TYPE::WITCH)	tempRenderModelType = RENDER_MODEL_TYPE::OTHER_PLAYER_WITCH;
		else std::cout << "�� �׷� �ϴ� ������! " << std::endl;

		otherPlayerContLock.lock(); //++++++++++++++++++++++++++++++++++++++++++++++++++1
		// �ȳ�! ���ο� �÷��̾�!

		std::cout << "[PUT] �÷��̾� (" << packet->key << ") ����!" << std::endl;

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
		else std::cout << "�� �׷� �ϴ� ������! " << std::endl;

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
		std::cout << "�� NPC ���µ� ���� NPC�� ���� Ű�� �����Ӹ� " << (int)packet->key << " // "<< std::bitset<32>(packet->key) << " // " << realKey << std::endl;
		break;
	default:
		std::cout << "��! �ϴ� ���� ���ڽ��� " << (int)packet->key << " // " << std::bitset<32>(packet->key) << " // " << realKey << std::endl;
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
	if (auto[objectType, realKey] = BIT_CONVERTER::WhatIsYourTypeAndRealKey(packet->key);
		objectType == OBJECT_TYPE::PLAYER)
	{
		if (realKey == myClientKey)
		{
			otherPlayerContLock.lock();
			otherPlayerCont.clear();
			otherPlayerContLock.unlock();

			monsterContLock.lock();
			monsterCont.clear();
			monsterContLock.unlock();

			isDeath = true;
		}
	}

	switch (auto[objectType, realKey] = BIT_CONVERTER::WhatIsYourTypeAndRealKey(packet->key); objectType)
	{
	case OBJECT_TYPE::PLAYER:
		std::cout << "[Remove] �÷��̾� (" << packet->key << ") �ȳ�!" << std::endl;

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

	switch (auto[objectType, realKey] = BIT_CONVERTER::WhatIsYourTypeAndRealKey(packet->key); objectType)
	{
	case OBJECT_TYPE::PLAYER:
		if (myClientKey == realKey)
		{
#ifdef _DEV_MODE_
			//std::cout << "�� ĳ���Ͱ� �̵��մϴ�. ��ġ x, y�� : " << packet.x << " " << packet.y << "\n";
#endif
			posX = packet->x;
			posY = packet->y;

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
	using namespace PACKET_DATA::MAIN_TO_CLIENT;
	Chat* packet = reinterpret_cast<Chat *>(pBufferStart);

	wprintf(L"%d : %s\n", (unsigned int)packet->key, packet->message);
}

void WGameFramework::RecvLoginFail(char* pBufferStart)
{
	using namespace PACKET_DATA::MAIN_TO_CLIENT;
	using namespace BIT_CONVERTER;

	LoginFail* packet = reinterpret_cast<LoginFail *>(pBufferStart);
	switch (packet->failReason)
	{
		// 0�̸� ���� ����, 1�̸� �̹� �α����� ����, 2�̸� ������� �ߴµ� �̹� �ִ� ����
	case 0:
		std::cout << "�ش� �г����� ������ �ִ� ������ �����ϴ�. Ȯ�� �� �ٽ� �õ��ϼ���. \n";
		break;
	case 1:
		std::cout << "�ش� ������ �̹� �α����� �����Դϴ�. \n";
		break;
	case 2:
		std::cout << "�ش� �г����� ������ �̹� �����մϴ�. �ٸ� �г������� ȸ�������ϼ���. \n";
		break;
	}

	networkManager->LogInOrSignUpProcess();
}

void WGameFramework::RecvStatChange(char* pBufferStart)
{
	using namespace PACKET_DATA::MAIN_TO_CLIENT;
	StatChange* packet = reinterpret_cast<StatChange *>(pBufferStart);

	switch (packet->changedStatType)
	{
	case STAT_CHANGE::HP: { hp = packet->newValue; break; }
	case STAT_CHANGE::MP: { mp = packet->newValue; break; }
	case STAT_CHANGE::LEVEL: { level = packet->newValue; break; }
	case STAT_CHANGE::EXP: { myExp = packet->newValue; break; }
	case STAT_CHANGE::RED_P: { redCount = packet->newValue; break; }
	case STAT_CHANGE::BLUE_P: { blueCount = packet->newValue; break; }
	case STAT_CHANGE::MONEY: { money = packet->newValue; break; }
	case STAT_CHANGE::MOVE_OK : { moveOK = packet->newValue; break; }
	case STAT_CHANGE::ATTACK_OK: { attackOK = packet->newValue; break; }
	case STAT_CHANGE::SKILL_1_OK: { skill1OK = packet->newValue; break; }
	case STAT_CHANGE::SKILL_2_OK: { skill2OK = packet->newValue; break; }
	}

	std::wcout << L"[���º���] : " << (int)packet->changedStatType << L"�� " << (int)packet->newValue << L"���� ����Ǿ����ϴ�." << std::endl;
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
