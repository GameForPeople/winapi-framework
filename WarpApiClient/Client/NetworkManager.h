#pragma once

class WGameFramework;
struct MemoryUnit;

class NetworkManager
{
public:
	NetworkManager(const std::string_view&, WGameFramework*);
	~NetworkManager();

	void SendMoveData(const BYTE inDirection);

private:
	void InitNetwork();

	// for Worker Thread
	static DWORD WINAPI StartWorkerThread(LPVOID arg);
	void WorkerThreadFunction();

	// for Send Recv
	void SendPacket(char* packetData);
	void RecvPacket();

	void AfterRecv(/*MemoryUnit* pClient,*/ int cbTransferred);
	void AfterSend(MemoryUnit* pMemoryUnit);

	void ProcessRecvData(int restSize);
	void ProcessLoadedPacket();

private:
	WGameFramework* pGameFramework;

	HANDLE hIOCP;
	std::thread workerThread;

	SOCKET socket;
	WSADATA wsa;
	SOCKADDR_IN serverAddr;

	std::string ipAddress;

	//MemoryUnit sendMemoryUnit;
	MemoryUnit* recvMemoryUnit;

	char loadedBuf[GLOBAL_DEFINE::MAX_SIZE_OF_RECV];
	int loadedSize;
};