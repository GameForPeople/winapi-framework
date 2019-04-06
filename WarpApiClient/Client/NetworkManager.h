#pragma once

class WGameFramework;
enum class PACKET_TYPE;

class NetworkManager
{
public:
	// Return 최적화 필요.
	std::pair<std::array<std::pair<UINT8, UINT8>, GLOBAL_DEFINE::MAX_CLIENT + 1>, int> SendVoidUpdate();	//PT_000
	std::pair<std::array<std::pair<UINT8, UINT8>, GLOBAL_DEFINE::MAX_CLIENT + 1>, int> SendMoveData(const DIRECTION);	// PT_001

	NetworkManager(const std::string_view&, WGameFramework* );
	~NetworkManager() = default;

private:
	void InitNetwork();
	
	void ProcessMyCharacterPosition(const int inPositionIndex);
	int ProcessOtherCharacterPosition(const int inStartIndex);

	// for Worker Thread
	static DWORD WINAPI StartWorkerThread(LPVOID arg);
	void WorkerThreadFunction();

private:
	WGameFramework* pGameFramework;

	HANDLE hIOCP;
	std::thread workerThread;

	SOCKET socket;
	WSADATA wsa;
	SOCKADDR_IN serverAddr;

	std::string ipAddress;
	char dataBuffer[RECV_BUFFER_SIZE];

	std::array<std::pair<UINT8, UINT8>, GLOBAL_DEFINE::MAX_CLIENT + 1> recvCharacterPoistionArr;
};