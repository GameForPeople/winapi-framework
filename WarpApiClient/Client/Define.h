#pragma once
/*
	kpu-warp-winapi-framework		Copyright �� https://github.com/KPU-WARP

	#0. �ش� ������Ʈ�� �ѱ��������� ���Ӱ��а� �м� �Ҹ��� WARP�� ���¼ҽ� WinAPI Framework�Դϴ�.
	#1. ���� ���� ������ ���� ����� ����̿��� Ȯ���Ͻ� �� �ֽ��ϴ�. 
		- https://github.com/KPU-WARP/winapi-framework
	
	#2. ��Ÿ ���Ǵ�, KoreaGameMaker@gmail.com���� ��Ź�帳�ϴ�. �����մϴ� :)
*/

/*
	Define.h
		- �ش� ��� ������, ������ Ŭ���̾�Ʈ�� �������� ����մϴ�.
		(Ŭ���̾�Ʈ���� ���� ������Ʈ �ڵ� ����)

	#0. �ش� �����, ������ Ŭ���̾�Ʈ

	!0. enum, ���� ���, ���� �Լ��� ���, �ݵ�� namespace�ȿ� ����Ǿ�� �մϴ�.
		- ����� ���ӽ����̽� ����, ���������� ���Ǵ� ��츦 �����մϴ�.
	!1. enum class, enum�� ��� �������� ENUM_SIZE�� �����ؾ��մϴ�.
*/

namespace GLOBAL_DEFINE
{
	constexpr USHORT MAIN_SERVER_PORT = 9000;
	constexpr USHORT CHAT_SERVER_PORT = 9001;
	constexpr USHORT MANAGER_SERVER_PORT = 9002;
	constexpr USHORT QUERY_SERVER_PORT = 9003;

	constexpr USHORT MAX_HEIGHT = 300;
	constexpr USHORT MAX_WIDTH = 300;

	constexpr BYTE ID_MAX_LEN = 10;
	constexpr BYTE ID_MAX_SIZE = ID_MAX_LEN * 2;

	constexpr BYTE CHAT_MAX_LEN = 50;
}

namespace STAT_CHANGE
{
	enum
	{
		HP,
		MP,
		LEVEL,
		EXP,
		RED_P,
		BLUE_P,
		MONEY,
		MOVE_OK,
		ATTACK_OK,
		SKILL_1_OK,
		SKILL_2_OK
	};
}

namespace NETWORK_TYPE
{
	enum /*class NETWORK_TYPE : BYTE */
	{
		CLIENT_RECV_FROM_MAIN,/* = 0*/
		CLIENT_RECV_FROM_CHAT,
		CLIENT_SEND_TO_MAIN,
		CLIENT_SEND_TO_CHAT,

		MAIN_RECV_FROM_CLIENT,
		MAIN_SEND_TO_CLIENT,

		CHAT_RECV_FROM_CLIENT,
		CHAT_SEND_TO_CLIENT,

		COMMAND_SEND_TO_CHAT,
		COMMAND_SEND_TO_MAIN,

		ENUM_SIZE
	};
}

namespace PACKET_TYPE
{
	namespace CLIENT_TO_MAIN
	{
		enum
		{
			MOVE, 	//LEFT, //UP, //RIGHT, //DOWN,
			LOGIN,
			SIGN_UP,
			ATTACK,
			USE_ITEM,
			CHAT,
			ENUM_SIZE
		};
	}

	namespace CLIENT_TO_CHAT
	{
		enum
		{
			CHAT,	// CS::CHAT�� SC::CHAT�� �����ؾ��մϴ�.
			CONNECT,	// ä�� ������ �ش� Ŭ���̾�Ʈ�� ����մϴ�.
			CHANGE,		// �ش� Ŭ���̾�Ʈ�� ���� ����Ǿ����ϴ�.
			ENUM_SIZE
		};
	}

	namespace MAIN_TO_CLIENT
	{
		enum
		{
			POSITION,
			LOGIN_OK,
			LOGIN_FAIL,
			PUT_PLAYER,
			REMOVE_PLAYER,
			CHAT,
			STAT_CHANGE,
			ENUM_SIZE
		};
	}

	namespace MAIN_TO_QUERY
	{
		enum
		{
			DEMAND_LOGIN = 0,
			DEMAND_SIGNUP = 1,
			SAVE_LOCATION = 2,
			SAVE_USERINFO
		};
	}

	namespace QUERY_TO_MAIN
	{
		enum
		{
			LOGIN_TRUE,
			LOGIN_FALSE,
			LOGIN_ALREADY,
			LOGIN_NEW
		};
	}

	namespace CHAT_TO_CLIENT
	{
		enum
		{
			CHAT,	// CS::CHAT�� SC::CHAT�� �����ؾ��մϴ�.
			URGENT_NOTICE,		// ChatServer���� Client�� ��� ������ ������.
			ENUM_SIZE
		};
	}

	namespace COMMAND_TO_CHAT
	{
		enum
		{
			URGENT_NOTICE,	// CommandServer���� ChatServer�� ��ް��� ��û�� ����.
			ENUM_SIZE
		};
	}
}

namespace PACKET_DATA
{
	using _PacketSizeType = const char;
	using _PacketTypeType = const char;	//? �̸��� �� �̵�����
	using _KeyType = unsigned int;	//? �̸��� �� �̵�����
	using _LevelType = unsigned int;
	using _PosType = unsigned short;
	using _CharType = WCHAR;
	using _ExpType = unsigned int;
	using _JobType = unsigned int;	//�ϴ�;
	using _HpType = unsigned int;
	using _MpType = unsigned int;
	using _MoneyType = unsigned int;
	using _RedCountType = unsigned int;
	using _BlueCountType = unsigned int;
	using _TreeCountType = unsigned int;

#pragma pack(push, 1)

	namespace CLIENT_TO_MAIN
	{
		struct Move {
			_PacketSizeType size;
			_PacketTypeType type;
			char direction;

			Move(const char inDirection) noexcept;
		};

		struct Login {
			_PacketSizeType size;
			_PacketTypeType type;
			_CharType id[GLOBAL_DEFINE::ID_MAX_LEN];

			Login(const _CharType* const pInNickname) noexcept;
		};

		struct SignUp {
			_PacketSizeType size;
			_PacketTypeType type;
			_CharType id[GLOBAL_DEFINE::ID_MAX_LEN];
			_JobType job;

			SignUp(const _CharType* const pInNickname, const _JobType) noexcept;
		};

		struct Attack {
			_PacketSizeType size;
			_PacketTypeType type;
			unsigned char attackType; //0�̸� �⺻����, 1�̸� ��ų1, 2�̸� ��ų2

			Attack(const unsigned char) noexcept;
		};

		struct Item {
			_PacketSizeType size;
			_PacketTypeType type;
			unsigned char useItemType;	//0�̸� ��������, 1�̸� �������

			Item(const unsigned char) noexcept;
		};

		struct Chat {
			_PacketSizeType size;
			_PacketTypeType type;
			_CharType message[GLOBAL_DEFINE::CHAT_MAX_LEN];

			Chat(_CharType*);
		};
	}

	namespace CLIENT_TO_CHAT
	{
		struct Chat {
#ifdef NOT_USE
			char size;	// Fixed - 1	0
			const char type;	// Fixed - 1	1
			char nickNameLength;	// 1	2
			char messageLength;	// 1	2	// �е���Ʈ��Ȱ������ ����1����Ʈ���ڰ��
			std::wstring nickName;	// 1	
			std::wstring message;

			//message[0] = Length;				//Fixed
			//message[1] = type;					//Fixed
			//message[2] = nickNameLength;
			//message[3] = messageLength;

			//message[4] ~message[4 + nickNameLength * 2] = Nickname;
			//message[5 + nickNameLength * 2 + 1] ~message[Length] = ChatMessage;

			Chat(const char* pBufferStart);
#endif
			char message[80];
			Chat(const std::wstring& inNickName, const std::wstring& inMessage);	// ���۷����� �ƴ�, Copy�մϴ�.
		};
	}

	namespace MAIN_TO_CLIENT
	{
		struct LoginOk
		{
			_PacketSizeType size;
			_PacketTypeType type;
			_KeyType key;
			//_CharType nickname[GLOBAL_DEFINE::ID_MAX_LEN];
			_PosType x;
			_PosType y;
			_LevelType level;
			_ExpType exp;
			_JobType job;
			_HpType hp;
			_MpType mp;
			_MoneyType money;
			_RedCountType redCount;
			_BlueCountType blueCount;
			_TreeCountType treeCount;

			LoginOk(const _KeyType, /*const _CharType* inNewNickname*/ const _PosType x, const _PosType y,
				_LevelType inlevel, _ExpType inExp, _JobType inJob, _HpType inHp, _MpType inMp,
				_MoneyType inMoney, _RedCountType inRedCount, _BlueCountType inBlueCount, _TreeCountType inTreeCount
			) noexcept;
		};

		struct LoginFail
		{
			_PacketSizeType size;
			_PacketTypeType type;
			const char failReason;	// 0�̸� ���� ����, 1�̸� �̹� �α����� ����, 2�̸� ������� �ߴµ� �̹� �ִ� ����

			LoginFail(const char inFailReason) noexcept;
		};

		struct PutPlayer
		{
			_PacketSizeType size;
			_PacketTypeType type;
			_KeyType key;
			_PosType x;
			_PosType y;
			_JobType job;

			PutPlayer(const _KeyType inMovedClientKey, const _PosType inX, const _PosType inY, const _JobType inJob) noexcept;
		};

		struct RemovePlayer
		{
			_PacketSizeType size;
			_PacketTypeType type;
			_KeyType key;

			RemovePlayer(const _KeyType inRemovedClientKey) noexcept;
		};

		struct Position
		{
			_PacketSizeType size;
			_PacketTypeType type;
			_KeyType key;
			_PosType x;
			_PosType y;

			Position(const _KeyType inMovedClientKey, const _PosType inX, const _PosType inY) noexcept;
		};

		struct Chat
		{
			_PacketSizeType size;
			_PacketTypeType type;
			_KeyType key;
			_CharType message[GLOBAL_DEFINE::CHAT_MAX_LEN];

			Chat(const _KeyType inSenderKey, const _CharType* const pInMessage) noexcept;
		};

		struct StatChange
		{
			_PacketSizeType size;
			_PacketTypeType type;
			char changedStatType;
			// 0 ü��, 1 ����, 2 ����, 3. ����ġ, 4. ����, 
			//5.�Ĺ�, 6.��, 7. �̵����ɿ���, 8. ���ݰ��ɿ���, 9. ��ų1���ɿ���, 10. ��ų2���ɿ���
			int newValue;

			StatChange(char /* STAT_CHANGE */, int) noexcept;
		};
	}

	namespace MAIN_TO_QUERY
	{
		struct DemandLogin
		{
			_PacketSizeType size;
			_PacketTypeType type;
			_KeyType key;	// ���߿� �ݳ��� ��, �� Ű�� �˷������.
			_CharType id[10];
			int		pw;

			DemandLogin(const _KeyType, const char* inId, const int);
		};

		struct DemandSignUp
		{
			_PacketSizeType size;
			_PacketTypeType type;
			_KeyType key;	// ���߿� �ݳ��� ��, �� Ű�� �˷������.
			_CharType id[10];
			_JobType job;

			DemandSignUp(const _KeyType, const char* inId, const _JobType);
		};

		struct SavePosition
		{
			_PacketSizeType size;
			_PacketTypeType type;
			// ����� �ܹ��⼺�̶� Key�� �ʿ����
			_CharType id[10];
			_PosType xPos;
			_PosType yPos;
			SavePosition(const _CharType * const, const _PosType, const _PosType);
		};

		struct SaveUserInfo
		{
			_PacketSizeType size;
			_PacketTypeType type;
			// ����� �ܹ��⼺�̶� Key�� �ʿ����
			int isOut;	// 0 �α׾ƿ���, 1 �����
			_CharType id[10];
			_PosType xPos;
			_PosType yPos;
			_LevelType level;
			_ExpType exp;
			_JobType job;
			_HpType hp;
			_MpType mp;
			_MoneyType money;
			_RedCountType redCount;
			_BlueCountType blueCount;
			_TreeCountType treeCount;

			SaveUserInfo(const int inIsOut, const _CharType* const inId, const _PosType inXPos, const _PosType inYPos,
				const _LevelType inLevel, const _ExpType exp, const _JobType job, const _HpType hp, const _MpType mp,
				const _MoneyType money, const _RedCountType redCount, const _BlueCountType blueCount, const _TreeCountType treeCount);
		};
	}

	namespace CHAT_TO_CLIENT
	{
		struct Chat
		{
			// ���ϸ� ���̱� ����, ä���� ������ ������� Ȱ��
			char message[80];
			Chat(char*);
		};
	}

	namespace QUERY_TO_MAIN
	{
		struct LoginTrue
		{
			_PacketSizeType size;
			_PacketTypeType type;
			_KeyType key;
			//_CharType nickname[10];
			_PosType xPos;
			_PosType yPos;
			_LevelType level;
			_ExpType exp;
			_JobType job;
			_HpType hp;
			_MpType mp;
			_MoneyType money;
			_RedCountType redCount;
			_BlueCountType blueCount;
			_TreeCountType treeCount;

			LoginTrue(const _KeyType/*, const _KeyType,  const _CharType *, const _PosType, const _PosType*/) noexcept;
		};

		struct LoginFail
		{
			_PacketSizeType size;
			_PacketTypeType type;
			_KeyType key;
			unsigned char failReason;

			LoginFail(const _KeyType, const unsigned char) noexcept;
		};

		struct LoginAlready
		{
			_PacketSizeType size;
			_PacketTypeType type;
			_KeyType key;
			_KeyType oldKey;

			LoginAlready(const _KeyType, const _KeyType) noexcept;
		};

		struct LoginNew
		{
			_PacketSizeType size;
			_PacketTypeType type;
			_KeyType key;
			_JobType job;

			LoginNew(const _KeyType, const _JobType) noexcept;
		};
	}

#pragma pack(pop)
}

namespace DIRECTION
{
	enum /* class DIRECTION : BYTE */
	{
		LEFT /*= 0*/,
		UP /*= 1*/,
		RIGHT /*= 2*/,
		DOWN /*= 3*/,
		ENUM_SIZE
	};
}

namespace JOB_TYPE
{
	enum /* : int */
	{
		KNIGHT = 1,
		ARCHER = 2,
		WITCH = 3,
		SLIME = 4,
		GOLEM = 5,
		DRAGON = 6
	};
}

namespace UNICODE_UTIL
{
	void SetLocaleToKorean();

	_NODISCARD std::string WStringToString(const std::wstring& InWstring);
	_NODISCARD std::wstring StringToWString(const std::string& InString);
}

namespace BIT_CONVERTER
{
	constexpr BYTE SEND_BYTE = (1 << 7);
	constexpr unsigned int NOT_PLAYER_INT = (1 << 31);	// 0�϶��� �÷��̾� ����Ʈ, 1�϶��� 2���˻� �ʿ�.
	constexpr unsigned int NPC_INT = (1 << 30);	// 0�� ���� ����, 1�� ���� NPC
	constexpr unsigned int REAL_INT = 0x3fffffff;	// 0, 1 ��Ʈ ����ũ�� ���� �� ����ϴ� ����.

	enum class OBJECT_TYPE : BYTE
	{
		PLAYER,
		MONSTER,
		NPC
	};

	std::pair<OBJECT_TYPE, unsigned int> WhatIsYourTypeAndRealKey(unsigned int) noexcept;
	unsigned int MakeMonsterKey(unsigned int) noexcept;

	/*_NODISCARD*/ BYTE MakeSendPacket(const BYTE inPacketType) noexcept;
	/*_NODISCARD*/ bool GetRecvOrSend(const char inChar) noexcept;

	/*_NODISCARD*/ BYTE MakeByteFromLeftAndRightByte(const BYTE inLeftByte, const BYTE inRightByte) noexcept;
	/*_NODISCARD*/ BYTE GetLeft4Bit(const BYTE inByte) noexcept;
	/*_NODISCARD*/ BYTE GetRight4Bit(const BYTE inByte) noexcept;
}