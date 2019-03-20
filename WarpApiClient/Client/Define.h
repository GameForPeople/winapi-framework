/*
	kpu-warp-winapi-framework		Copyright �� https://github.com/KPU-WARP

	#0. �ش� ������Ʈ�� �ѱ��������� ���Ӱ��а� �м� �Ҹ��� WARP�� ���¼ҽ� WinAPI Framework�Դϴ�.
	#1. ���� ���� ������ ���� ����� ����̿��� Ȯ���Ͻ� �� �ֽ��ϴ�. 
		- https://github.com/KPU-WARP/winapi-framework
	
	#2. ��Ÿ ���Ǵ�, KoreaGameMaker@gmail.com���� ��Ź�帳�ϴ�. �����մϴ� :)
*/

#pragma once

#ifdef _DEBUG
	#define _DEV_MODE_
#endif

#define		_NORETURN			[[noreturn]]
#define		_NODISCARD			[[nodiscard]]
#define		_DEPRECATED			[[deprecated]]
#define		_MAYBE_UNUSED		[[maybe_unused]]
#define		_FALLTHROUGH		[[fallthrough]]

#define		_INLINE				__inline

namespace GLOBAL_DEFINE
{
	constexpr UINT8	MAIN_TIMER{ 1 };
	constexpr UINT8 MAIN_TIMER_FRAME{ 17 };

	constexpr UINT16 FRAME_WIDTH{ 1000 };
	constexpr UINT16 FRAME_HEIGHT{ 800 };

	constexpr UINT8 MAX_LOADSTRING{ 100 };

	constexpr USHORT MAX_OTHER_CLIENT{ 10 };
}

// Direction
enum class DIRECTION : BYTE
{
	LEFT = 0,
	UP = 1,
	RIGHT = 2,
	DOWN = 3
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

