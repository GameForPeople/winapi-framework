/*
	kpu-warp-winapi-framework		Copyright �� https://github.com/KPU-WARP

	#0. �ش� ������Ʈ�� �ѱ��������� ���Ӱ��а� �м� �Ҹ��� WARP�� ���¼ҽ� WinAPI Framework�Դϴ�.
	#1. ���� ���� ������ ���� ����� ����̿��� Ȯ���Ͻ� �� �ֽ��ϴ�.
		- https://github.com/KPU-WARP/winapi-framework

	#2. ��Ÿ ���Ǵ�, KoreaGameMaker@gmail.com���� ��Ź�帳�ϴ�. �����մϴ� :)
*/

#include "stdafx.h"

namespace GLOBAL_UTIL
{
	namespace BIT_CONVERTER
	{
		// Ŭ��� �״�� ���, ������ ���� �ζ����ϱ�.

		/*
			GetLeft4Bit
				- HIWORD�� �����ϰ� �����մϴ�. �ϳ��� ����Ʈ�� �޾Ƽ� ����(����) 4���� ��Ʈ�� ����Ʈ�� ��ȯ�ؼ� ��ȯ�մϴ�.
		*/
		_NODISCARD BYTE GetLeft4Bit(const BYTE inByte) noexcept
		{
			return (inByte >> 4) & (0x0f);
		}

		/*
			GetRight4Bit
				- LOWORD(?)�� �����ϰ� �����մϴ�. �ϳ��� ����Ʈ�� �޾Ƽ� ����(����) 4���� ��Ʈ�� ����Ʈ�� ��ȯ�ؼ� ��ȯ�մϴ�.
		*/
		_NODISCARD BYTE GetRight4Bit(const BYTE inByte) noexcept
		{
			return (inByte) & (0x0f);
		}
	}

	namespace ERROR_HANDLING
	{
		_NORETURN void ERROR_QUIT(const WCHAR *msg)
		{
			LPVOID lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				WSAGetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0,
				NULL
			);

			MessageBox(NULL, (LPTSTR)lpMsgBuf, msg, MB_ICONERROR);
			LocalFree(lpMsgBuf);
			exit(1);
		};

		/*_DEPRECATED*/ void ERROR_DISPLAY(const WCHAR *msg)
		{
			LPVOID lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				WSAGetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0,
				NULL
			);

			std::cout << "[ERROR : " << msg << "] " << (LPTSTR)&lpMsgBuf;
			LocalFree(lpMsgBuf);
		};
	}
}
