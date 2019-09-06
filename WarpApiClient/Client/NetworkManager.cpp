#include "stdafx.h"

#include "ClientDefine.h"
#include "Define.h"

#include "MemoryUnit.h"

#include "GameFrameWork.h"
#include "NetworkManager.h"

NetworkManager::NetworkManager(const std::string_view& inIPAddress, WGameFramework* InGameFramework)
	: ipAddress(inIPAddress) 
	, pGameFramework(InGameFramework)
	, hIOCP()
	, workerThread()
	, wsa()
	, socket()
	, serverAddr()
	, recvMemoryUnit(nullptr)
	, loadedBuf()
	, loadedSize()
{
	//for (int i = 0; i < 10; ++i)
	//{
	//	recvCharacterPoistionArr[i] = {0, 0};
	//}
	ERROR_HANDLING::errorRecvOrSendArr[0] = ERROR_HANDLING::HandleRecvOrSendError;
	ERROR_HANDLING::errorRecvOrSendArr[1] = ERROR_HANDLING::NotError;

	recvMemoryUnit = new MemoryUnit(true);

	InitNetwork();
}

NetworkManager::~NetworkManager()
{
	workerThread.join();

	pGameFramework = nullptr;

	delete recvMemoryUnit;
}

void NetworkManager::InitNetwork()
{
	using namespace GLOBAL_UTIL::ERROR_HANDLING;

	// 1. ���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) ERROR_QUIT(L"WSAStartup()");

	// 2. ����� �Ϸ� ��Ʈ ����
	if (hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)
		; hIOCP == NULL) ERROR_QUIT(TEXT("Make_WorkerThread()"));

	// 3. ��Ĺ ����
	if (this->socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)
		; socket == INVALID_SOCKET) ERROR_QUIT(L"Create_Socket()");

	// 4. ���ϰ� ����� �Ϸ� ��Ʈ ����
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket), hIOCP, socket, 0);

	// 5. Ŭ���̾�Ʈ ���� ����ü ��ü ����.
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
	serverAddr.sin_port = htons(GLOBAL_DEFINE::MAIN_SERVER_PORT);

	// 6. ��Ŀ ������ ���� �� IOCP ���
	workerThread = std::thread{ StartWorkerThread, (LPVOID)this };

	// 7. Ŀ��Ʈ!!!!!!!!! ���ھƾƾƾƾƾƾƾ�!!!!!!!
	if (int retVal = connect(socket, (SOCKADDR*)& serverAddr, sizeof(serverAddr))
		; retVal == SOCKET_ERROR) ERROR_QUIT(L"bind()");

	std::cout << "[CONNECT] ������ ���������� ����Ǿ����ϴ�." << std::endl;

	LogInOrSignUpProcess();

	// 8. ���ú� ��!
	RecvPacket();
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

		if (retVal == 0 || cbTransferred == 0)
		{
			// �ƴ�! Ŭ���̾�Ʈ���� ��Ʈ��ũ ������ ���ٰ�??
			std::cout << "[����] ��Ʈ��ũ ������ �߻��߽��ϴ�. Ŭ���̾�Ʈ�� �����մϴ�." << std::endl;
			
			if (retVal == 0)
			{
				std::cout << "[����] ���� ������ retVal == 0�Դϴ�." << std::endl;
			}
			else
			{
				std::cout << "[����] ���� ������ cbTransferred == 0�Դϴ�." << std::endl;
			}
			
			ERROR_HANDLING::ERROR_QUIT(L"GetQueuedCompletionStatus()");

			break;
		}

		pMemoryUnit->isRecv == true
			? AfterRecv(cbTransferred)
			: AfterSend(pMemoryUnit);
	}
}

/*
	SendPacket()
		- WSASend!�� ���⿡���� ������ �� �ֽ��ϴ�.

	!0. �ܼ��� WSA Send�� �ִ� �Լ��Դϴ�. �����ʹ� �غ����ּ���.
	!1. �� �Լ��� ȣ���ϱ� ����, wsaBuf�� len�� �������ּ���.

	?0. wsaBuf�� buf�� ���������� �ٲ���� �ұ��?
*/
void NetworkManager::SendPacket(char* packetData)
{
	MemoryUnit* sendMemoryUnit = new MemoryUnit(false);
	memcpy(sendMemoryUnit->dataBuf, packetData, packetData[0]);
	sendMemoryUnit->wsaBuf.len = packetData[0];

	DWORD flag{};
	ZeroMemory(&sendMemoryUnit->overlapped, sizeof(sendMemoryUnit->overlapped));

	ERROR_HANDLING::errorRecvOrSendArr[
		static_cast<bool>(
			1 + WSASend(socket, &sendMemoryUnit->wsaBuf, 1, NULL, 0, &sendMemoryUnit->overlapped, NULL)
			)
	]();
}

/*
	RecvPacket()
		- WSA Recv�� ���⼭�� �����մϴ�.

	!0. SocketInfo�� �ִ� wsaBuf -> buf �� ���ú긦 �մϴ�.
	!1. len�� ������ ���� ����մϴ�. MAX_SIZE_OF_RECV_BUFFER!
*/
void NetworkManager::RecvPacket()
{
	// ���� �����Ϳ� ���� ó���� ������ �ٷ� �ٽ� ���� �غ�.
	DWORD flag{};
	
	ZeroMemory(&(recvMemoryUnit->overlapped), sizeof(recvMemoryUnit->overlapped));
	ERROR_HANDLING::errorRecvOrSendArr[
		static_cast<bool>(
			1 + WSARecv(socket, &(recvMemoryUnit->wsaBuf), 1, NULL, &flag /* NULL*/, &(recvMemoryUnit->overlapped), NULL)
			)
	]();
}

/*
	NetworkManager::AfterRecv(SocketInfo* pClient)
		- ���ú� �Լ� ȣ�� ��, Ŭ���̾�Ʈ�� �����͸� ������ �޾��� ��, ȣ��Ǵ� �Լ�.
*/
void NetworkManager::AfterRecv(/*MemoryUnit* pClient,*/ int cbTransferred)
{
	//std::cout << "��Ŷ ���� size : " << (int)loadedBuf[0] << " Type : " << (int)loadedBuf[1] << std::endl;

	// ���� ������ ó��
	ProcessRecvData(cbTransferred);

	RecvPacket();
}

/*
	NetworkManager::ProcessRecvData(SocketInfo* pClient, int restSize)
		- ���� �����͵��� ��Ŷȭ�Ͽ� ó���ϴ� �Լ�.
*/
void NetworkManager::ProcessRecvData(int restSize)
{
	char* pBuf = recvMemoryUnit->dataBuf; // pBuf -> ó���ϴ� ���ڿ��� ���� ��ġ
	char packetSize{ 0 }; // ó���ؾ��� ��Ŷ�� ũ��

	// ������ ó���� ��ġ�� ���� ���۰� �ִٸ�, ó���ؾ��� ��Ŷ ����� �˷���.
	if (0 < loadedSize) packetSize = loadedBuf[0];

	// ó������ ���� ������ ũ�Ⱑ 0�� �ɶ����� �����ϴ�.
	while (restSize > 0)
	{
		// ������ ó���� ��ġ�� ���� ���۸� ó���ؾ��Ѵٸ� �н�, �ƴ϶�� ó���ؾ��� ��Ŷ�� ũ�⸦ ����.
		if (packetSize == 0) packetSize = pBuf[0];

		// ó���ؾ��ϴ� ��Ŷ ������ �߿���, ������ �̹� ó���� ��Ŷ ����� ���ش�.
		int required = packetSize - loadedSize;

		// ��Ŷ�� �ϼ��� �� ���� �� (��û�ؾ��� �������, ���� ����� ũ�ų� ���� ��)
		if (restSize >= required)
		{
			memcpy(loadedBuf + loadedSize, pBuf, required);
			//---

			ProcessLoadedPacket();

			//---
			loadedSize = 0;
			restSize -= required;
			pBuf += required;
			packetSize = 0;
		}
		// ��Ŷ�� �ϼ��� �� ���� ��
		else
		{
			memcpy(loadedBuf + loadedSize, pBuf, restSize);
			loadedSize += restSize;
			break;
			//restSize = 0;
		}
	}
}

/*
	NetworkManager::ProcessLoadedPacket()
		- ���� �����͵��� ��Ŷȭ�Ͽ� ó���ϴ� �Լ�.
*/
void NetworkManager::ProcessLoadedPacket()
{
	using namespace PACKET_TYPE;

	switch (loadedBuf[1])
	{
	case MAIN_TO_CLIENT::LOGIN_OK:
		pGameFramework->RecvLoginOK(loadedBuf);
		break;
	case MAIN_TO_CLIENT::PUT_PLAYER:
		pGameFramework->RecvPutPlayer(loadedBuf);
		break;
	case MAIN_TO_CLIENT::REMOVE_PLAYER:
		pGameFramework->RecvRemovePlayer(loadedBuf);
		break;
	case MAIN_TO_CLIENT::POSITION:
		pGameFramework->RecvPosition(loadedBuf);
		break;
	case MAIN_TO_CLIENT::LOGIN_FAIL:
		pGameFramework->RecvLoginFail(loadedBuf);
		break;
	case MAIN_TO_CLIENT::CHAT:
		pGameFramework->RecvChat(loadedBuf);
		break;
	case MAIN_TO_CLIENT::STAT_CHANGE:
		pGameFramework->RecvStatChange(loadedBuf);
		break;
	default:
		std::cout << "[RECV] ���ǵ��� ���� ���������� �޾ҽ��ϴ�. Ȯ�����ּ���. " << loadedBuf[1] << "\n";
		break;
	}
}

/*
	GameServer::AfterSend(SocketInfo* pClient)
		- WSASend �Լ� ȣ�� ��, ������ ������ ������ ��, ȣ��Ǵ� �Լ�.
*/
void NetworkManager::AfterSend(MemoryUnit* pMemoryUnit)
{
	delete pMemoryUnit;
}

void NetworkManager::SendMoveData(const BYTE /*DIRECTION*/ inDirection)
{
#ifdef _DEV_MODE_
	std::cout << "[SEND] �����͸� �����մϴ�. ���� Ű���� : MOVE,  ������" << (int)inDirection << "\n";
#endif
	PACKET_DATA::CLIENT_TO_MAIN::Move packet(inDirection);
	SendPacket(reinterpret_cast<char*>(&packet));
}

void NetworkManager::SendAttack(const unsigned char inAttackType)
{
	PACKET_DATA::CLIENT_TO_MAIN::Attack packet(inAttackType);
	SendPacket(reinterpret_cast<char*>(&packet));
}

void NetworkManager::SendItem(const unsigned char inItemType)
{
	PACKET_DATA::CLIENT_TO_MAIN::Item packet(inItemType);
	SendPacket(reinterpret_cast<char*>(&packet));
}

void NetworkManager::LogInOrSignUpProcess()
{
	std::cout << "�α����� 1��, ȸ�������� 2���� �Է����ּ��� : " << std::endl;

	int tempInputtedCommand{};

	std::cin >> tempInputtedCommand;

	switch (tempInputtedCommand)
	{
	case 1:
	{
		std::cout << "ID�� �Է����ּ��� : " << std::endl;

		WCHAR tempID[9]{};
		wscanf(L"%s", tempID);

		PACKET_DATA::CLIENT_TO_MAIN::Login loginPacket(tempID);
		SendPacket(reinterpret_cast<char*>(&loginPacket));
		break;
	}
	case 2:
	{
		std::cout << "ȸ�������� ���Ͻô� ID�� �Է����ּ��� : " << std::endl;

		WCHAR tempID[9]{};
		wscanf(L"%s", tempID);

		std::cout << "���Ͻô� ������ �������ּ���. \n   1. ��� 2. �ü� 3. ����" << std::endl;
		int tempInputtedJob{};
		std::cin >> tempInputtedJob;

		if (tempInputtedJob > 0 && tempInputtedJob < 4)
		{
			PACKET_DATA::CLIENT_TO_MAIN::SignUp signUpPacket(tempID, tempInputtedJob);
			SendPacket(reinterpret_cast<char*>(&signUpPacket));
		}
		else
		{
			std::cout << "�׷����� �����! �߰���!" << std::endl;
			throw ERROR;
		}
		break;
	}
	}
}
