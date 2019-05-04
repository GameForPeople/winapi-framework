#pragma once
/*
	kpu-warp-winapi-framework		Copyright �� https://github.com/KPU-WARP

	#0. �ش� ������Ʈ�� �ѱ��������� ���Ӱ��а� �м� �Ҹ��� WARP�� ���¼ҽ� WinAPI Framework�Դϴ�.
	#1. ���� ���� ������ ���� ����� ����̿��� Ȯ���Ͻ� �� �ֽ��ϴ�. 
		- https://github.com/KPU-WARP/winapi-framework
	
	#2. ��Ÿ ���Ǵ�, KoreaGameMaker@gmail.com���� ��Ź�帳�ϴ�. �����մϴ� :)
*/

#pragma once

/*
	Define.h
		- �ش� ��� ������, ������ Ŭ���̾�Ʈ�� �������� ����մϴ�.
		(Ŭ���̾�Ʈ���� ���� ������Ʈ �ڵ� ����)

	#0. �ش� �����, ������ Ŭ���̾�Ʈ

	!0. enum, ���� ���, ���� �Լ��� ���, �ݵ�� namespace�ȿ� ����Ǿ�� �մϴ�.
		- ����� ���ӽ����̽� ����, ���������� ���Ǵ� ��츦 �����մϴ�.
	!1. enum class, enum�� ��� �������� ENUM_SIZE�� �����ؾ��մϴ�.
*/

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
			PUT_PLAYER,
			REMOVE_PLAYER,
			ENUM_SIZE
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
	using _KeyType = unsigned int;
	using _PosType = unsigned short;

#pragma pack(push, 1)

	namespace CLIENT_TO_MAIN
	{
		struct Move {
			const char size;
			const char type;
			char direction;

			Move(char inDirection) noexcept;
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
			const char size;
			const char type;
			_KeyType id;

			LoginOk(const _KeyType inNewId) noexcept;
		};

		struct PutPlayer
		{
			const char size;
			const char type;
			_KeyType id;
			_PosType x;
			_PosType y;

			PutPlayer(const _KeyType inMovedClientId, const _PosType inX, const _PosType inY) noexcept;
		};

		struct RemovePlayer
		{
			const char size;
			const char type;
			_KeyType id;

			RemovePlayer(const _KeyType inRemovedClientID) noexcept;
		};

		struct Position
		{
			const char size;
			const char type;
			_KeyType id;
			_PosType x;
			_PosType y;

			Position(const _KeyType inMovedClientId, const _PosType inX, const _PosType inY) noexcept;
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

namespace UNICODE_UTIL
{
	void SetLocaleToKorean();

	_NODISCARD std::string WStringToString(const std::wstring& InWstring);
	_NODISCARD std::wstring StringToWString(const std::string& InString);
}

namespace GLOBAL_DEFINE
{
	constexpr USHORT MAIN_SERVER_PORT = 9000;
	constexpr USHORT CHAT_SERVER_PORT = 9001;
	constexpr USHORT MANAGER_SERVER_PORT = 9002;

	constexpr USHORT MAX_HEIGHT = 800;
	constexpr USHORT MAX_WIDTH = 800;
}

enum class OBJECT_TYPE : BYTE
{
	PLAYER,
	MONSTER,
	NPC
};

namespace BIT_CONVERTER
{
	constexpr BYTE SEND_BYTE = (1 << 7);
	constexpr unsigned int NOT_PLAYER_INT = (1 << 31);	// 0�϶��� �÷��̾� ����Ʈ, 1�϶��� 2���˻� �ʿ�.
	constexpr unsigned int NPC_INT = (1 << 30);	// 0�� ���� ����, 1�� ���� NPC
	constexpr unsigned int REAL_INT = 0x3fffffff;	// 0�� ���� ����, 1�� ���� NPC

	std::pair<OBJECT_TYPE, unsigned int> WhatIsYourTypeAndRealKey(unsigned int) noexcept;

	/*_NODISCARD*/ BYTE MakeSendPacket(const BYTE inPacketType) noexcept;
	/*_NODISCARD*/ bool GetRecvOrSend(const char inChar) noexcept;

	/*_NODISCARD*/ BYTE MakeByteFromLeftAndRightByte(const BYTE inLeftByte, const BYTE inRightByte) noexcept;
	/*_NODISCARD*/ BYTE GetLeft4Bit(const BYTE inByte) noexcept;
	/*_NODISCARD*/ BYTE GetRight4Bit(const BYTE inByte) noexcept;
}