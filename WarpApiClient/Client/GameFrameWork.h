/*
	kpu-warp-winapi-framework		Copyright ⓒ https://github.com/KPU-WARP

	#0. 해당 프로젝트는 한국산업기술대 게임공학과 학술 소모임 WARP의 오픈소스 WinAPI Framework입니다.
	#1. 관련 세부 정보는 깃헙 저장소 리드미에서 확인하실 수 있습니다.
		- https://github.com/KPU-WARP/winapi-framework

	#2. 기타 문의는, KoreaGameMaker@gmail.com으로 부탁드립니다. 감사합니다 :)
*/

#pragma once

class NetworkManager;

class StretchModel;
class TransparentModel;

class BaseActor;
class Pawn;

class WGameFramework
{
	using _ClientKeyType = int;
	using _OtherClientCont = std::list<std::pair<_ClientKeyType, std::unique_ptr<Pawn>>>;

public:
	WGameFramework(const std::string_view&);
	~WGameFramework();

	void Reset();
	void Clear();

	void Create(HWND hWnd);
	void OnDraw(HDC hdc);
	void OnUpdate(const float frameTime = 0.17f);
	void KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam);
	void Mouse(UINT iMessage, WPARAM wParan, LPARAM lParam);

public:	// Called in NetworkManager
	void RecvLoginOK(const char*);
	void RecvPutPlayer(const char*);
	void RecvRemovePlayer(const char*);
	void RecvPosition(const char*);

private:
	//void SetCharactersLocation(const std::pair<std::array<std::pair<UINT8, UINT8>, GLOBAL_DEFINE::MAX_CLIENT + 1>, int> inCont, const bool inMyCharacterInclude);

private:
	HWND	m_hWnd;
	BYTE	tickCount;

private:
	std::unique_ptr<NetworkManager> networkManager;

private:
	TransparentModel* originPlayerModel;
	TransparentModel* originOtherPlayerModel;
	StretchModel* backgroundModel;

	std::unique_ptr<BaseActor> backgroundActor;
	std::unique_ptr<Pawn> playerCharacter;

	_OtherClientCont otherPlayerCont;
	std::mutex otherPlayerContLock;	// 이 바보같은 락 나중에 뿌셔버리기

private:
	_ClientKeyType myClientKey;
};