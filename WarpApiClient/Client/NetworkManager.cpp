#include "stdafx.h"

#include "ClientDefine.h"
#include "Define.h"

#include "MemoryUnit.h"

#include "GameFrameWork.h"
#include "NetworkManager.h"

NetworkManager::NetworkManager(const std::string_view& inIPAddress, WGameFramework* InGameFramework)
	: ipAddress(inIPAddress), pGameFramework(InGameFramework)
	//, sendMemoryUnit(false)
	, recvMemoryUnit(nullptr)
	, hIOCP()
	, loadedBuf()
	, loadedSize()
	, serverAddr()
	, socket()
	, wsa()
{
	//for (int i = 0; i < 10; ++i)
	//{
	//	recvCharacterPoistionArr[i] = {0, 0};
	//}

	recvMemoryUnit = new MemoryUnit(true);

	InitNetwork();
}

NetworkManager::~NetworkManager()
{
	pGameFramework = nullptr;
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
	workerThread = std::thread{ StartWorkerThread, (LPVOID)this };

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

		if (retVal == 0 || cbTransferred == 0)
		{
			// �ƴ�! Ŭ���̾�Ʈ���� ��Ʈ��ũ ������ ���ٰ�??
			std::cout << "[����] ��Ʈ��ũ ������ �߻��߽��ϴ�. Ŭ���̾�Ʈ�� �����մϴ�." << std::endl;
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
	// ���� ������ ó��
	ProcessRecvData(cbTransferred);
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

			//---
			restSize -= required;
			pBuf += required;
			packetSize = 0;
		}
		// ��Ŷ�� �ϼ��� �� ���� ��
		else
		{
			memcpy(loadedBuf, pBuf, restSize);
			loadedSize = restSize;
			restSize = 0;
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
	case SC::LOGIN_OK:
		pGameFramework->RecvLoginOK(loadedBuf);
		break;
	case SC::PUT_PLAYER:
		pGameFramework->RecvPutPlayer(loadedBuf);
		break;
	case SC::REMOVE_PLAYER:
		pGameFramework->RecvRemovePlayer(loadedBuf);
		break;
	case SC::POSITION:
		pGameFramework->RecvPosition(loadedBuf);
		break;
	default:
		std::cout << "���ǵ��� ���� ���������� �޾ҽ��ϴ�. \n";
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

}