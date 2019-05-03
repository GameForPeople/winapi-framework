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
			char id;

			LoginOk(const char inNewId) noexcept;
		};

		struct PutPlayer
		{
			const char size;
			const char type;
			char id;
			char x;
			char y;

			PutPlayer(const char inMovedClientId, const char inX, const char inY) noexcept;
		};

		struct RemovePlayer
		{
			const char size;
			const char type;
			char id;

			RemovePlayer(const char inRemovedClientID) noexcept;
		};

		struct Position
		{
			const char size;
			const char type;
			char id;
			char x;
			char y;

			Position(const char inMovedClientId, const char inX, const char inY) noexcept;
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