#include "stdafx.h"
#include "Define.h"

namespace PACKET_DATA
{
	namespace CLIENT_TO_MAIN
	{
		Move::Move(const char inDirection) noexcept :
			size(sizeof(Move)), type(PACKET_TYPE::CLIENT_TO_MAIN::MOVE),
			direction(inDirection)
		{}

		Login::Login(const _CharType* const pInID) noexcept :
			size(sizeof(Login)), type(PACKET_TYPE::CLIENT_TO_MAIN::LOGIN),
			id()
		{
			lstrcpynW(id, pInID, GLOBAL_DEFINE::ID_MAX_SIZE);
		}

		SignUp::SignUp(const _CharType* const pInNickname, const _JobType inJob) noexcept :
			size(sizeof(SignUp)), type(PACKET_TYPE::CLIENT_TO_MAIN::SIGN_UP),
			id(), job(inJob)
		{
			lstrcpynW(id, pInNickname, GLOBAL_DEFINE::ID_MAX_SIZE);
		}

		Attack::Attack(const unsigned char attackType) noexcept :
			size(sizeof(Attack)), type(PACKET_TYPE::CLIENT_TO_MAIN::ATTACK),
			attackType(attackType)
		{
		}

		Item::Item(const unsigned char useItemType) noexcept :
			size(sizeof(Item)), type(PACKET_TYPE::CLIENT_TO_MAIN::USE_ITEM),
			useItemType(useItemType)
		{
		}

		Chat::Chat(_CharType* pInMessage) :
			size(sizeof(Chat)), type(PACKET_TYPE::CLIENT_TO_MAIN::CHAT),
			message()
		{
			lstrcpynW(message, pInMessage, GLOBAL_DEFINE::CHAT_MAX_LEN);
		}
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
			const BYTE nickNameLength = static_cast<const BYTE>(inNickName.size() * 2); // BYTE
			const BYTE messageLength = static_cast<const BYTE>(inMessage.size() * 2);	//BYTE

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
		LoginOk::LoginOk(const _KeyType inKey, /*const _CharType* inNewNickname*/ const _PosType x, const _PosType y,
			_LevelType inlevel, _ExpType inExp, _JobType inJob, _HpType inHp, _MpType inMp,
			_MoneyType inMoney, _RedCountType inRedCount, _BlueCountType inBlueCount, _TreeCountType inTreeCount) noexcept :

			size(sizeof(LoginOk)), type(PACKET_TYPE::MAIN_TO_CLIENT::LOGIN_OK),
			key(inKey),
			//nickname(),
			x(x), y(y),
			level(inlevel),
			exp(inExp),
			job(inJob),
			hp(inHp),
			mp(inMp),
			money(inMoney),
			redCount(inRedCount),
			blueCount(inBlueCount),
			treeCount(inTreeCount)
		{
			//memcpy(nickname, inNewNickname, GLOBAL_DEFINE::ID_MAX_SIZE);
		}

		LoginFail::LoginFail(const char inFailReason) noexcept :
			size(sizeof(LoginFail)), type(PACKET_TYPE::MAIN_TO_CLIENT::LOGIN_FAIL),
			failReason(inFailReason)
		{}

		PutPlayer::PutPlayer(const _KeyType inMovedClientKey, const _PosType inX, const _PosType inY, const _JobType inJob) noexcept :
			size(sizeof(PutPlayer)), type(PACKET_TYPE::MAIN_TO_CLIENT::PUT_PLAYER),
			key(inMovedClientKey),
			x(inX),
			y(inY),
			job(inJob)
		{}

		RemovePlayer::RemovePlayer(const _KeyType inRemovedClientKey) noexcept :
			size(sizeof(RemovePlayer)), type(PACKET_TYPE::MAIN_TO_CLIENT::REMOVE_PLAYER),
			key(inRemovedClientKey)
		{}

		Position::Position(const _KeyType inMovedClientId, const _PosType inX, const _PosType inY) noexcept :
			size(sizeof(Position)), type(PACKET_TYPE::MAIN_TO_CLIENT::POSITION),
			key(inMovedClientId),
			x(inX),
			y(inY)
		{}

		Chat::Chat(const _KeyType inSenderKey, const _CharType* const pInMessage) noexcept :
			size(sizeof(Chat)), type(PACKET_TYPE::MAIN_TO_CLIENT::CHAT),
			key(inSenderKey),
			message()
		{
			lstrcpynW(message, pInMessage, GLOBAL_DEFINE::CHAT_MAX_LEN);
		}

		StatChange::StatChange(char /* STAT_CHANGE */ inStatType, int inNewValue) noexcept :
			size(sizeof(StatChange)), type(PACKET_TYPE::MAIN_TO_CLIENT::STAT_CHANGE),
			changedStatType(inStatType), newValue(inNewValue)
		{
		}
	}

	namespace MAIN_TO_QUERY
	{
		DemandLogin::DemandLogin(const _KeyType inKey, const char* inId, int inPw)
			: size(sizeof(DemandLogin)), type(PACKET_TYPE::MAIN_TO_QUERY::DEMAND_LOGIN),
			key(inKey), id(), pw(inPw)
		{
			memcpy(id, inId, GLOBAL_DEFINE::ID_MAX_SIZE);
		}

		DemandSignUp::DemandSignUp(const _KeyType inKey, const char* inId, const _JobType inJob)
			: size(sizeof(DemandSignUp)), type(PACKET_TYPE::MAIN_TO_QUERY::DEMAND_SIGNUP),
			key(inKey), id(), job(inJob)
		{
			memcpy(id, inId, GLOBAL_DEFINE::ID_MAX_SIZE);
		}

		SavePosition::SavePosition(const _CharType* const inId, const _PosType inXPos, const _PosType inYPos)
			: size(sizeof(SavePosition)), type(PACKET_TYPE::MAIN_TO_QUERY::SAVE_LOCATION),
			id(), xPos(inXPos), yPos(inYPos)
		{
			lstrcpynW(id, inId, GLOBAL_DEFINE::ID_MAX_SIZE);
		}

		SaveUserInfo::SaveUserInfo(const int inIsOut, const _CharType* const inId, const _PosType inXPos, const _PosType inYPos,
			const _LevelType inLevel, const _ExpType inExp, const _JobType inJob, const _HpType inHp, const _MpType inMp,
			const _MoneyType inMoney, const _RedCountType inRedCount, const _BlueCountType inBlueCount, const _TreeCountType inTreeCount)
			: size(sizeof(SaveUserInfo)), type(PACKET_TYPE::MAIN_TO_QUERY::SAVE_USERINFO),
			isOut(inIsOut), id(), xPos(inXPos), yPos(inYPos), level(inLevel), exp(inExp), job(inJob), hp(inHp), mp(inMp),
			money(inMoney), redCount(inRedCount), blueCount(inBlueCount), treeCount(inTreeCount)
		{
			lstrcpynW(id, inId, GLOBAL_DEFINE::ID_MAX_SIZE);
		}
	}

	namespace CHAT_TO_CLIENT
	{
		Chat::Chat(char* inPtr)
		{
			memcpy(message, inPtr, static_cast<BYTE>(inPtr[0]));
		}
	}

	namespace QUERY_TO_MAIN
	{
		LoginTrue::LoginTrue(const _KeyType inKey /*, const _CharType* inID, const _PosType inX, const _PosType inY*/) noexcept :
			size(sizeof(LoginTrue)), type(PACKET_TYPE::QUERY_TO_MAIN::LOGIN_TRUE),
			key(inKey)
		{
		}

		LoginFail::LoginFail(const _KeyType inKey, const unsigned char inFailReason) noexcept :
			size(sizeof(LoginFail)), type(PACKET_TYPE::QUERY_TO_MAIN::LOGIN_FALSE),
			key(inKey),
			failReason(inFailReason)
		{}

		LoginAlready::LoginAlready(const _KeyType inKey, const _KeyType oldKey) noexcept :
			size(sizeof(LoginAlready)), type(PACKET_TYPE::QUERY_TO_MAIN::LOGIN_ALREADY),
			key(inKey),
			oldKey(oldKey)
		{}

		LoginNew::LoginNew(const _KeyType inKey, const _JobType inJob) noexcept :
			size(sizeof(LoginNew)), type(PACKET_TYPE::QUERY_TO_MAIN::LOGIN_NEW),
			key(inKey),
			job(inJob)
		{}
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
		if (inKey < NOT_PLAYER_INT) retPair.first = OBJECT_TYPE::PLAYER;
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