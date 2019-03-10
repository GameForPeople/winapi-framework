/*
	kpu-warp-winapi-framework		Copyright ⓒ https://github.com/KPU-WARP

	#0. 해당 프로젝트는 한국산업기술대 게임공학과 학술 소모임 WARP의 오픈소스 WinAPI Framework입니다.
	#1. 관련 세부 정보는 깃헙 저장소 리드미에서 확인하실 수 있습니다.
		- https://github.com/KPU-WARP/winapi-framework

	#2. 기타 문의는, KoreaGameMaker@gmail.com으로 부탁드립니다. 감사합니다 :)
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
	// 편한 디버깅 환경을 제공하기 위해, 개발 모드일 때, 콘솔창을 켜줍니다.
#ifdef _DEV_MODE_
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif

	std::cout << "현재 DEV MODE가 활성화 되었습니다. \n";
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
