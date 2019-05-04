/*
	kpu-warp-winapi-framework		Copyright ⓒ https://github.com/KPU-WARP

	#0. 해당 프로젝트는 한국산업기술대 게임공학과 학술 소모임 WARP의 오픈소스 WinAPI Framework입니다.
	#1. 관련 세부 정보는 깃헙 저장소 리드미에서 확인하실 수 있습니다.
		- https://github.com/KPU-WARP/winapi-framework

	#2. 기타 문의는, KoreaGameMaker@gmail.com으로 부탁드립니다. 감사합니다 :)
*/

#pragma once

class NetworkManager;
class RenderModelManager;

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

	void Create(HWND hWnd);
	void OnDraw(HDC hdc);
	void OnUpdate(const float frameTime = 0.17f);
	void KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam);
	void Mouse(UINT iMessage, WPARAM wParan, LPARAM lParam);

public:	// Called in NetworkManager
	void RecvLoginOK(char*);
	void RecvPutPlayer(char*);
	void RecvRemovePlayer(char*);
	void RecvPosition(char*);
	void RecvChat(char*);

private:
	void UpdateBackgroundActor();
	void UpdateOtherObject();

private:
	HWND	m_hWnd;

private:
	std::unique_ptr<NetworkManager> networkManager;
	std::unique_ptr<RenderModelManager> renderModelManager;

private:
	std::unique_ptr<Pawn> playerCharacter;
	std::vector<std::vector<std::unique_ptr<Pawn>>> backgroundActorCont;
	
	_OtherClientCont otherPlayerCont;
	std::mutex otherPlayerContLock;	// 이 바보같은 락 나중에 백스페이스로 고냥 뿌셔버리기

	_OtherClientCont monsterCont;
	std::mutex monsterContLock;	

	std::unique_ptr<BaseActor> coverUI;
	std::unique_ptr<BaseActor> broadcastAreaUI;

private:
	std::string ipAddress;
	_ClientKeyType myClientKey;
};