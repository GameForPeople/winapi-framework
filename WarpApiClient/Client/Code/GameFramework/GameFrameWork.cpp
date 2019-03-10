/*
	kpu-warp-winapi-framework		Copyright �� https://github.com/KPU-WARP

	#0. �ش� ������Ʈ�� �ѱ��������� ���Ӱ��а� �м� �Ҹ��� WARP�� ���¼ҽ� WinAPI Framework�Դϴ�.
	#1. ���� ���� ������ ���� ����� ����̿��� Ȯ���Ͻ� �� �ֽ��ϴ�.
		- https://github.com/KPU-WARP/winapi-framework

	#2. ��Ÿ ���Ǵ�, KoreaGameMaker@gmail.com���� ��Ź�帳�ϴ�. �����մϴ� :)
*/

#include "stdafx.h"

#include "../Object/Pawn.h"

#include "../RenderModel/StretchModel.h"
#include "../RenderModel/TransparentModel.h"

#include "../GameFramework/GameFrameWork.h"

WGameFramework::WGameFramework() noexcept
	: m_hWnd()
	, originPlayerModel(nullptr)
	, backgroundModel(nullptr)
	, backgroundActor(nullptr)
	, playerCharacter(nullptr)
{
	// ���� ����� ȯ���� �����ϱ� ����, ���� ����� ��, �ܼ�â�� ���ݴϴ�.
#ifdef _DEV_MODE_
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif

	std::cout << "���� DEV MODE�� Ȱ��ȭ �Ǿ����ϴ�. \n";
#endif
}

WGameFramework::~WGameFramework()
{
	delete originPlayerModel;
	delete backgroundModel;

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
	backgroundModel = new StretchModel(L"Resource/Image/Image_Background.png");
	
	playerCharacter = std::make_unique<Pawn>(originPlayerModel,
		RenderData(0, 0, 100, 95, RGB(255, 0, 0)));
	
	backgroundActor = std::make_unique<BaseActor>(backgroundModel, 
		RenderData(0,0, 1000, 770));
}

void WGameFramework::OnDraw(HDC hdc)
{
	backgroundActor->Render(hdc);
	playerCharacter->Render(hdc);
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

				case static_cast<WPARAM>(VK_UP) :
				case static_cast<WPARAM>(VK_DOWN) :
				case static_cast<WPARAM>(VK_LEFT) :
				case static_cast<WPARAM>(VK_RIGHT) :
					playerCharacter->MoveWithDirection(static_cast<DIRECTION>((static_cast<BYTE>(wParam) - VK_LEFT)));
					break;
			}
			break;

		case WM_KEYUP:
			break;

		default:
			break;
	}
}
