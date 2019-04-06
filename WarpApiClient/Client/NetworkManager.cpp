#include "stdafx.h"

#include "Define.h"

#include "MemoryUnit.h"

#include "GameFrameWork.h"
#include "NetworkManager.h"

NetworkManager::NetworkManager(const std::string_view& inIPAddress, WGameFramework* InGameFramework)
	: ipAddress(inIPAddress), pGameFramework(InGameFramework)
{
	for (int i = 0; i < 10; ++i)
	{
		recvCharacterPoistionArr[i] = {0, 0};
	}

	InitNetwork();
}

void NetworkManager::InitNetwork()
{
	using namespace GLOBAL_UTIL::ERROR_HANDLING;

	// 1. ���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) ERROR_QUIT(L"WSAStartup()");

	// 2. ����� �Ϸ� ��Ʈ ����
	if (hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)
		; hIOCP == NULL) ERROR_QUIT(TEXT("Make_WorkerThread()"));

	// 3. ��Ŀ ������ ���� �� IOCP ���
	workerThread = std::thread{ };

	// 4. ��Ĺ ����
	if (this->socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)
		; socket == INVALID_SOCKET) ERROR_QUIT(L"Create_Socket()");

	// 5. Ŭ���̾�Ʈ ���� ����ü ��ü ����.
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
	serverAddr.sin_port = htons(GLOBAL_DEFINE::SERVER_PORT);

	// 6. Ŀ��Ʈ!!!!!!!!! ���ھƾƾƾƾƾƾƾ�!!!!!!!
	if (int retVal = connect(socket, (SOCKADDR*)& serverAddr, sizeof(serverAddr))
		; retVal == SOCKET_ERROR) ERROR_QUIT(L"bind()");
}

DWORD WINAPI NetworkManager::StartWorkerThread(LPVOID arg)
{
	NetworkManager* pNetworkManager = static_cast<NetworkManager*>(arg);
	pNetworkManager->WorkerThreadFunction();

	return 0;
};

void NetworkManager::WorkerThreadFunction()
{
	int retVal{};
	DWORD cbTransferred;
	unsigned long long clientKey;
	MemoryUnit* pMemoryUnit;

	while (7)
	{
		retVal = GetQueuedCompletionStatus(
			hIOCP,
			&cbTransferred,
			&clientKey,
			reinterpret_cast<LPOVERLAPPED*>(&pMemoryUnit),
			INFINITE
		);

	}
}



std::pair<std::array<std::pair<UINT8, UINT8>, GLOBAL_DEFINE::MAX_CLIENT + 1>, int> NetworkManager::SendVoidUpdate()
{
	dataBuffer[0] = static_cast<BYTE>(PACKET_TYPE::VOID_UPDATE);
	dataBuffer[1] = 0x00;	/* Void */

	if (int retVal = send(socket, dataBuffer, NetworkManager::SEND_BUFFER_SIZE, NULL)
		; retVal == SOCKET_ERROR)	GLOBAL_UTIL::ERROR_HANDLING::ERROR_QUIT(L"Send�� �����Ͽ����ϴ�. Ŭ���̾�Ʈ�� �����մϴ�. \n");

	{
		//std::cout << "[Send] Void Packet�� �����߽��ϴ�. : \n";
	}

	if (int retVal = recv(socket, dataBuffer, NetworkManager::RECV_BUFFER_SIZE, NULL)
		; retVal == SOCKET_ERROR)	GLOBAL_UTIL::ERROR_HANDLING::ERROR_QUIT(L"Recv�� �����Ͽ����ϴ�. Ŭ���̾�Ʈ�� �����մϴ�. \n");
	
	{
		//std::cout << "[RECV] Packet�� ���۹޾ҽ��ϴ�. : 100 \n\n";
	}
	const int recvOtherPlayerCount = ProcessOtherCharacterPosition(1);
	return std::make_pair(recvCharacterPoistionArr, recvOtherPlayerCount);
}

std::pair<std::array<std::pair<UINT8, UINT8>, GLOBAL_DEFINE::MAX_CLIENT + 1>, int> NetworkManager::SendMoveData(const DIRECTION inDirection)
{
	dataBuffer[0] = static_cast<BYTE>(PACKET_TYPE::MOVE);
	dataBuffer[1] = static_cast<BYTE>(inDirection);

	if (int retVal = send(socket, dataBuffer, NetworkManager::SEND_BUFFER_SIZE, NULL)
		; retVal == SOCKET_ERROR)	GLOBAL_UTIL::ERROR_HANDLING::ERROR_QUIT(L"Send�� �����Ͽ����ϴ�. Ŭ���̾�Ʈ�� �����մϴ�. \n");

	{
		std::cout << "[Send] Move Packet�� �����߽��ϴ�. : " << [/*void*/](const DIRECTION dir) noexcept -> std::string
		{
			if (dir == DIRECTION::LEFT) return "LEFT";
			if (dir == DIRECTION::RIGHT) return "RIGHT";
			if (dir == DIRECTION::UP) return "UP";
			if (dir == DIRECTION::DOWN) return "DOWN";

		}(inDirection)
			<< "\n";
	}

	if (int retVal = recv(socket, dataBuffer, NetworkManager::RECV_BUFFER_SIZE, NULL)
		; retVal == SOCKET_ERROR)	GLOBAL_UTIL::ERROR_HANDLING::ERROR_QUIT(L"Recv�� �����Ͽ����ϴ�. Ŭ���̾�Ʈ�� �����մϴ�. \n");

	{
		std::cout << "[RECV] Packet�� ���۹޾ҽ��ϴ�. : " << (int)(dataBuffer[0]) << "���� : " << std::bitset<8>(dataBuffer[1])
			<< " X : " << (int)(GLOBAL_UTIL::BIT_CONVERTER::GetLeft4Bit(dataBuffer[1]))
			<< " , y : " << (int)(GLOBAL_UTIL::BIT_CONVERTER::GetRight4Bit(dataBuffer[1]))
			<< "\n 2 : " << std::bitset<8>(dataBuffer[2])
			<< "\n 3 : " << std::bitset<8>(dataBuffer[3])
			<< "\n 4 : " << std::bitset<8>(dataBuffer[4])
			<< "\n 5 : " << std::bitset<8>(dataBuffer[5])
			<< "\n 6 : " << std::bitset<8>(dataBuffer[6])
			<< "\n 7 : " << std::bitset<8>(dataBuffer[7])
			<< "\n 8 : " << std::bitset<8>(dataBuffer[8])
			<< "\n 9 : " << std::bitset<8>(dataBuffer[9])
			<< "\n 10 : " << std::bitset<8>(dataBuffer[10])
			<< "\n 11 : " << std::bitset<8>(dataBuffer[11])
			<< "\n 12 : " << std::bitset<8>(dataBuffer[12]) << "\n\n";
	}

	ProcessMyCharacterPosition(1);
	const int recvOtherPlayerCount = ProcessOtherCharacterPosition(2);
	return std::make_pair(recvCharacterPoistionArr, recvOtherPlayerCount);
}

void NetworkManager::ProcessMyCharacterPosition(const int inPositionIndex)
{
	recvCharacterPoistionArr[0] = { GLOBAL_UTIL::BIT_CONVERTER::GetLeft4Bit(dataBuffer[inPositionIndex]), GLOBAL_UTIL::BIT_CONVERTER::GetRight4Bit(dataBuffer[inPositionIndex]) };
}

int NetworkManager::ProcessOtherCharacterPosition(const int inStartIndex)
{
	const int recvConnectedPlayerCount = static_cast<int>(dataBuffer[inStartIndex]);
	for (int i = 1; i < recvConnectedPlayerCount; ++i)
	{
		recvCharacterPoistionArr[i] = { GLOBAL_UTIL::BIT_CONVERTER::GetLeft4Bit(dataBuffer[i + inStartIndex]), GLOBAL_UTIL::BIT_CONVERTER::GetRight4Bit(dataBuffer[i + inStartIndex]) };
	}

	return recvConnectedPlayerCount;
}