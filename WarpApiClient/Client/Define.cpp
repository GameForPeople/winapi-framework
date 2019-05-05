#include "stdafx.h"

#include "Define.h"

namespace PACKET_DATA
{
	namespace CLIENT_TO_MAIN
	{
		Move::Move(char inDirection) noexcept :
			size(sizeof(Move)), type(PACKET_TYPE::CLIENT_TO_MAIN::MOVE),
			direction(inDirection)
		{}
	}

	namespace CLIENT_TO_CHAT
	{
#ifdef NOT_USE
		Chat::Chat(const char* inRecvBuffer) :
			size(inRecvBuffer[0]), type(PACKET_TYPE::CS::CHAT_SERVER_CHAT),
			nickNameLength(inRecvBuffer[2]),
			nickName(),
			message()
		{
			assert(false, L"�� ������ Chat�� ���� �Ⱦ�����! �ٸ� ���� ī���͸� ���� ���ܵӽô�.");
			std::string stringNickname(inRecvBuffer[3], inRecvBuffer[3 + nickNameLength]);
			nickName = std::move(UNICODE_UTIL::StringToWString(stringNickname));

			std::string stringMessage(inRecvBuffer[3 + nickNameLength + 1],
				inRecvBuffer[size - 1]);
			message = std::move(UNICODE_UTIL::StringToWString(stringMessage));
		}
#endif
		Chat::Chat(const std::wstring& inNickName, const std::wstring& inMessage)
		{
			const BYTE nickNameLength = inNickName.size() * 2; // BYTE
			const BYTE messageLength = inMessage.size() * 2;	//BYTE

			message[0] = nickNameLength + messageLength + 4;
			message[1] = PACKET_TYPE::CLIENT_TO_CHAT::CHAT;
			message[2] = nickNameLength;
			message[3] = messageLength;

			memcpy(message + 4, UNICODE_UTIL::WStringToString(/*const_cast<std::wstring&>*/(inNickName)).c_str(), nickNameLength);
			memcpy(message + (4 + nickNameLength), UNICODE_UTIL::WStringToString(inMessage).c_str(), messageLength);
		}
	}

	namespace MAIN_TO_CLIENT
	{
		LoginOk::LoginOk(const UINT inNewId) noexcept :
			size(sizeof(LoginOk)), type(PACKET_TYPE::MAIN_TO_CLIENT::LOGIN_OK),
			id(inNewId)
		{}

		PutPlayer::PutPlayer(const UINT inPutClientId, const USHORT inX, const USHORT inY) noexcept :
			size(sizeof(PutPlayer)), type(PACKET_TYPE::MAIN_TO_CLIENT::PUT_PLAYER),
			id(inPutClientId),
			x(inX),
			y(inY)
		{}

		RemovePlayer::RemovePlayer(const UINT inRemovedClientID) noexcept :
			size(sizeof(RemovePlayer)), type(PACKET_TYPE::MAIN_TO_CLIENT::REMOVE_PLAYER),
			id(inRemovedClientID)
		{}

		Position::Position(const UINT inMovedClientId, const USHORT inX, const USHORT inY) noexcept :
			size(sizeof(Position)), type(PACKET_TYPE::MAIN_TO_CLIENT::POSITION),
			id(inMovedClientId),
			x(inX),
			y(inY)
		{}
	}

	namespace CHAT_TO_CLIENT
	{
		Chat::Chat(char* inPtr)
		{
			memcpy(message, inPtr, static_cast<BYTE>(inPtr[0]));
		}
	}
}

namespace UNICODE_UTIL
{
	void SetLocaleToKorean()
	{
		_wsetlocale(LC_ALL, L"Korean");

		// ? ��� ���� ���� �ϴ� ������.
		//26444 �� ������, ���� �ʿ� ����, L-Value�� �������ϴ°�;
		/*auto oldLocale = std::wcout.imbue(std::locale("koeran")); */
	}

	_NODISCARD std::string WStringToString(const std::wstring& InWString)
	{
		const int sizeBuffer = WideCharToMultiByte(CP_ACP, 0, &InWString[0], -1, NULL, 0, NULL, NULL);

		std::string retString(sizeBuffer, 0);

		WideCharToMultiByte(CP_ACP, 0, &InWString[0], -1, &retString[0], sizeBuffer, NULL, NULL);

		// FixError ==
		retString.pop_back(); //(retString.end(), retString.end());
		//retString.insert(retString.end(), '\0');

		return retString;
	}

	_NODISCARD std::wstring StringToWString(const std::string& InString)
	{
		const int sizeBuffer = MultiByteToWideChar(CP_ACP, 0, &InString[0], -1, NULL, 0);

		std::wstring retString(sizeBuffer, 0);

		MultiByteToWideChar(CP_ACP, 0, &InString[0], -1, &retString[0], sizeBuffer);

		return retString;
	}
}

namespace BIT_CONVERTER
{
	std::pair<OBJECT_TYPE, unsigned int> WhatIsYourTypeAndRealKey(unsigned int inKey) noexcept
	{
		std::pair<OBJECT_TYPE, unsigned int> retPair;

		//if ((inKey & NOT_PLAYER_INT) == inKey) retPair.first = OBJECT_TYPE::PLAYER;
		//else if ((inKey & NPC_INT) != inKey) retPair.first = OBJECT_TYPE::MONSTER;
		//else retPair.first = OBJECT_TYPE::NPC;
		if(inKey < NOT_PLAYER_INT) retPair.first = OBJECT_TYPE::PLAYER;
		else  if ((inKey < NOT_PLAYER_INT + NPC_INT)) retPair.first = OBJECT_TYPE::MONSTER;
		else  retPair.first = OBJECT_TYPE::NPC;

		retPair.second = inKey & REAL_INT;

		return retPair;
	}

	unsigned int MakeMonsterKey(unsigned int inOnlyIndex) noexcept
	{
		return (inOnlyIndex | NOT_PLAYER_INT);
	}

	/*
		MakeByteFromLeftAndRightByte()
			- ���ڷ� ���� ��Ŷ Ÿ��(����Ʈ)�� �ֻ��� ��Ʈ�� 1�� �ٲ��� ��ȯ�մϴ�.

		!0. ��Ŷ Ÿ�� ������, ox7f���� Ŭ ���, �ش� �Լ� �� ���� ���� ������ ������ �߻��մϴ�.
	*/
	BYTE MakeSendPacket(const BYTE inPacketType) noexcept { return inPacketType | SEND_BYTE; }

	/*
		MakeByteFromLeftAndRightByte()
			- ���ڷ� ���� ��Ŷ Ÿ��(����Ʈ)�� �ֻ��� ��Ʈ�� �˻��մϴ�.

		#0. �ֻ��� ��Ʈ�� 1�� ���, true��, 0�� ��� false�� ��ȯ�մϴ�. (����ȯ�� ���� Array Overflow ���� )

		!0. ��Ŷ Ÿ�� ������, ox7f���� Ŭ ���, �ش� �Լ� �� ���� ���� ������ ������ �߻��մϴ�.
	*/
	bool GetRecvOrSend(const char inChar) noexcept { return (inChar >> 7) & (0x01); }

	/*
		MakeByteFromLeftAndRightByte
			- 0x10���� ���� ����Ʈ �ΰ��� ���ڷ� �޾�(Left, Right) ���� 4����Ʈ�� Left, ���� 4�� ��Ʈ�� Right�� ��ƹ�ȯ�մϴ�.

		!0. ���ڷ� ������ �� ����Ʈ ���, 0x10���� ���� ���� ���;��մϴ�.
			- ���ڷ� ������ Left ����Ʈ�� 0x0f���� ū ���� ���� ���, �����÷ο�Ǿ� ���������� ���� ��Ⱥ�� �� ����.
			- ���ڷ� ������ Right ����Ʈ�� 0x0f���� ū ���� ���� ���, LeftByte�� | ���꿡�� ���������� ���� ��ȯ�� �� ����.
	*/
	BYTE MakeByteFromLeftAndRightByte(const BYTE inLeftByte, const BYTE inRightByte) noexcept
	{
		return (inLeftByte << 4) | (inRightByte);
	}

	/*
		GetLeft4Bit
			- HIWORD(?)�� �����ϰ� �����մϴ�. �ϳ��� ����Ʈ�� �޾Ƽ� ����(����) 4���� ��Ʈ�� ����Ʈ�� ��ȯ�ؼ� ��ȯ�մϴ�.
	*/
	BYTE GetLeft4Bit(const BYTE inByte) noexcept
	{
		return (inByte >> 4) & (0x0f);
	}

	/*
		GetRight4Bit
			- LOWORD(?)�� �����ϰ� �����մϴ�. �ϳ��� ����Ʈ�� �޾Ƽ� ����(����) 4���� ��Ʈ�� ����Ʈ�� ��ȯ�ؼ� ��ȯ�մϴ�.
	*/
	BYTE GetRight4Bit(const BYTE inByte) noexcept
	{
		return (inByte) & (0x0f);
	}
}