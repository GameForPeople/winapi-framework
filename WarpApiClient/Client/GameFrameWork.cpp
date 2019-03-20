/*
	kpu-warp-winapi-framework		Copyright �� https://github.com/KPU-WARP

	#0. �ش� ������Ʈ�� �ѱ��������� ���Ӱ��а� �м� �Ҹ��� WARP�� ���¼ҽ� WinAPI Framework�Դϴ�.
	#1. ���� ���� ������ ���� ����� ����̿��� Ȯ���Ͻ� �� �ֽ��ϴ�.
		- https://github.com/KPU-WARP/winapi-framework

	#2. ��Ÿ ���Ǵ�, KoreaGameMaker@gmail.com���� ��Ź�帳�ϴ�. �����մϴ� :)
*/

#include "stdafx.h"

#include "NetworkManager.h"

#include "Pawn.h"
#include "TransparentModel.h"
#include "StretchModel.h"

#include "GameFramework.h"

WGameFramework::WGameFramework(const std::string_view& inIPAddress)
	: networkManager(std::make_unique<NetworkManager>(inIPAddress))
	, tickCount(0)
{
}

WGameFramework::~WGameFramework()
{
	delete originPlayerModel;
	delete backgroundModel;
	delete originOtherPlayerModel;

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
	
	playerCharacter = std::make_unique<Pawn>(originPlayerModel,
		RenderData(0, 0, 100, 100, RGB(255, 0, 0)));
	
	for( int i = 0; i < otherPlayerArr.size(); ++i)
		otherPlayerArr[i] = std::make_unique<Pawn>(originOtherPlayerModel,
			RenderData(0, 0, 100, 100, RGB(255, 0, 0)), false);
	
	backgroundActor = std::make_unique<BaseActor>(backgroundModel, 
		RenderData(0,0, 1000, 770));
}

void WGameFramework::OnDraw(HDC hdc)
{
	// Caution! Render Sort!

	backgroundActor->Render(hdc);

	for (auto iter = otherPlayerArr.cbegin(); iter != otherPlayerArr.cend(); ++iter) (*iter)->Render(hdc);

	playerCharacter->Render(hdc);
}

void WGameFramework::OnUpdate(const float frameTime)
{
	if (++tickCount == 30)
	{
		SetCharactersLocation(networkManager->SendVoidUpdate(), false);
		tickCount = 0;
	}
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

				case static_cast<WPARAM>(VK_UP) :
				case static_cast<WPARAM>(VK_DOWN) :
				case static_cast<WPARAM>(VK_LEFT) :
				case static_cast<WPARAM>(VK_RIGHT) :
					// ����� ���ŷ�̸�, Send �� Recv ���� ����.
					SetCharactersLocation(networkManager->SendMoveData(static_cast<DIRECTION>(static_cast<BYTE>(wParam) - VK_LEFT)), true);
				break;
			}
			break;

		case WM_KEYUP:
			break;

		default:
			break;
	}
}

void WGameFramework::SetCharactersLocation(const std::pair<std::array<std::pair<UINT8, UINT8>, GLOBAL_DEFINE::MAX_OTHER_CLIENT + 1>, int> inCont, const bool inMyCharacterInclude)
{
	auto[pRecvCharacterPositionArr, connectedPlayerCount] = inCont;

	if(inMyCharacterInclude)
		playerCharacter->SetPosition(pRecvCharacterPositionArr[0]);

	for (int i = 0; i < connectedPlayerCount - 1; ++i)
	{
		otherPlayerArr[i]->SetPosition(pRecvCharacterPositionArr[i + 1]);
		otherPlayerArr[i]->SetRender(true);
	}
	for (int i = connectedPlayerCount - 1; i < GLOBAL_DEFINE::MAX_OTHER_CLIENT + 1; ++i) otherPlayerArr[i]->SetRender(false);
}
