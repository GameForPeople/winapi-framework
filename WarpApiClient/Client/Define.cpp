#include "stdafx.h"

#include "Define.h"

namespace PACKET_DATA
{
	namespace CLIENT_TO_SERVER
	{
		Move::Move(char inDirection) noexcept :
			size(sizeof(Move)), type(PACKET_TYPE::CS::MOVE),
			direction(inDirection)
		{}
	}

	namespace SERVER_TO_CLIENT
	{
		LoginOk::LoginOk(const char inNewId) noexcept :
			size(sizeof(LoginOk)), type(PACKET_TYPE::SC::LOGIN_OK),
			id(inNewId)
		{}

		PutPlayer::PutPlayer(const char inPutClientId, const char inX, const char inY) noexcept :
			size(sizeof(PutPlayer)), type(PACKET_TYPE::SC::PUT_PLAYER),
			id(inPutClientId),
			x(inX),
			y(inY)
		{}

		RemovePlayer::RemovePlayer(const char inRemovedClientID) noexcept :
			size(sizeof(RemovePlayer)), type(PACKET_TYPE::SC::REMOVE_PLAYER),
			id(inRemovedClientID)
		{}

		Position::Position(const char inMovedClientId, const char inX, const char inY) noexcept :
			size(sizeof(Position)), type(PACKET_TYPE::SC::POSITION),
			id(inMovedClientId),
			x(inX),
			y(inY)
		{}
	}
}

namespace UNICODE_UTIL
{
	void SetLocaleToKorean()
	{
		_wsetlocale(LC_ALL, L"korean");

		//26444 �� ������, ���� �ʿ� ����, L-Value�� �������ϴ°�;
		auto oldLocale = std::wcout.imbue(std::locale("koeran"));
	}
}