/*
	kpu-warp-winapi-framework		Copyright ⓒ https://github.com/KPU-WARP

	#0. 해당 프로젝트는 한국산업기술대 게임공학과 학술 소모임 WARP의 오픈소스 WinAPI Framework입니다.
	#1. 관련 세부 정보는 깃헙 저장소 리드미에서 확인하실 수 있습니다. 
		- https://github.com/KPU-WARP/winapi-framework
	
	#2. 기타 문의는, KoreaGameMaker@gmail.com으로 부탁드립니다. 감사합니다 :)
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

