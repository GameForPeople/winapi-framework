/*
	kpu-warp-winapi-framework		Copyright ⓒ https://github.com/KPU-WARP

	#0. 해당 프로젝트는 한국산업기술대 게임공학과 학술 소모임 WARP의 오픈소스 WinAPI Framework입니다.
	#1. 관련 세부 정보는 깃헙 저장소 리드미에서 확인하실 수 있습니다.
		- https://github.com/KPU-WARP/winapi-framework

	#2. 기타 문의는, KoreaGameMaker@gmail.com으로 부탁드립니다. 감사합니다 :)
*/

#pragma once

class StretchModel;
class TransparentModel;

class BaseActor;
class Pawn;

class WGameFramework
{
public:
	WGameFramework();
	~WGameFramework();

	void Reset();
	void Clear();

	void Create(HWND hWnd);
	void OnDraw(HDC hdc);
	void OnUpdate(const float frameTime = 0.17f);
	void KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam);
	void Mouse(UINT iMessage, WPARAM wParan, LPARAM lParam);

private:
	HWND	m_hWnd;

private:
	TransparentModel* originPlayerModel;
	StretchModel* backgroundModel;

	std::unique_ptr<BaseActor> backgroundActor;
	std::unique_ptr<Pawn> playerCharacter;
};

