
// Copyright �� https://github.com/KPU-WARP
// ���� ���� �� ����� ����
// �ڵ� ��� �� KoreaGameMaker@gmail.com �����Ͻñ� �ٶ��ϴ�.

#include "stdafx.h"
#include "GameFrameWork.h"


WGameFrameWork::WGameFrameWork()
{
	Clear();
}

WGameFrameWork::~WGameFrameWork()
{
}

void WGameFrameWork::Reset()
{
}

void WGameFrameWork::Clear()
{
}

void WGameFrameWork::Create(HWND hWnd)
{
	m_hWnd = hWnd;
}

void WGameFrameWork::OnDraw(HDC hdc)
{
	static int x = 10;
	x++;
	Rectangle(hdc, x + 10, 10, x + 100, 100);
}

void WGameFrameWork::OnUpdate(const float frameTime)
{
}

void WGameFrameWork::KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		case WM_KEYDOWN:
		{
			if (wParam == VK_Q)
			{
				SendMessage(m_hWnd, WM_DESTROY, 0, 0);
				return;
			}
		}
		break;
		

		case WM_KEYUP:
		{

		}
		break;


	}

}
