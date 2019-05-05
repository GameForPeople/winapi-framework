#pragma once

namespace GLOBAL_DEFINE
{
	constexpr UINT8	MAIN_TIMER{ 1 };
	constexpr UINT8 MAIN_TIMER_FRAME{ 17 };

	constexpr UINT16 FRAME_WIDTH{ 1000 };
	constexpr UINT16 FRAME_HEIGHT{ 830 };

	constexpr UINT16 PLAY_FRAME_HALF_WIDTH{ 350 };
	constexpr UINT16 PLAY_FRAME_HALF_HEIGHT{ 350 };

	constexpr UINT8 MAX_LOADSTRING{ 100 };

	constexpr USHORT MAX_SIZE_OF_RECV{ 100 };		//Recv �ѹ��� ���� �� �ִ� �ִ� ������
	constexpr USHORT MAX_SIZE_OF_SEND{ 10 };		//sizeof(PACKET_DATA::CLIENT_TO_SERVER::Down);		//Recv �ѹ��� ���� �� �ִ� �ִ� ������

	constexpr UINT8 BLOCK_WIDTH_SIZE{ 40 };
	constexpr UINT8 BLOCK_HEIGHT_SIZE{ 40 };
}

namespace ERROR_HANDLING {
	// �ش� static Function Array�� �ʱ�ȭ�� NetworkManager
	static std::function<void(void)> errorRecvOrSendArr[2];

	inline void NotError(void) {};
	void HandleRecvOrSendError(void);

	_NORETURN void ERROR_QUIT(const WCHAR* msg);
	/*_DEPRECATED*/ void ERROR_DISPLAY(const CHAR* msg);
}

namespace COLOR
{
	constexpr DWORD _RED = RGB(255, 0, 0);
	constexpr DWORD _GREEN = RGB(0, 255, 0);
	constexpr DWORD _BLUE = RGB(0, 0, 255);
	constexpr DWORD _WHITE = RGB(255, 255, 255);
	constexpr DWORD _BLACK = RGB(0, 0, 0);
}

enum class RENDER_MODEL_TYPE : BYTE
{
	PLAYER = 0x00,
	OTHER_PLAYER = 0x01,
	BACKGROUND = 0x02,
	COVER_UI = 0x03,
	BROADCAST_UI = 0x04,
	MONSTER = 0x05,
	ENUM_SIZE
};

// KeyBoard
enum class VK_KEY
{
	VK_0 = 48,
	VK_1 = 49,
	VK_2,
	VK_3,
	VK_4,
	VK_5,
	VK_6,
	VK_7,
	VK_8,
	VK_9,
	VK_A = 0x41,
	VK_B,
	VK_C,
	VK_D,
	VK_E,
	VK_F,
	VK_G,
	VK_H,
	VK_I,
	VK_J,
	VK_K,
	VK_L,
	VK_M,
	VK_N,
	VK_O,
	VK_P,
	VK_Q,
	VK_R,
	VK_S,
	VK_T,
	VK_U,
	VK_V,
	VK_W,
	VK_X,
	VK_Y,
	VK_Z
};