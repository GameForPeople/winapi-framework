#pragma once

enum class PACKET_TYPE;

class NetworkManager
{
	static constexpr USHORT	SERVER_PORT = 9000;
	static constexpr USHORT RECV_BUFFER_SIZE = 13;
	static constexpr USHORT	SEND_BUFFER_SIZE = 2;

public:
	// Return 최적화 필요.
	std::pair<std::array<std::pair<UINT8, UINT8>, GLOBAL_DEFINE::MAX_OTHER_CLIENT + 1>, int> SendVoidUpdate();	//PT_000
	std::pair<std::array<std::pair<UINT8, UINT8>, GLOBAL_DEFINE::MAX_OTHER_CLIENT + 1>, int> SendMoveData(const DIRECTION);	// PT_001

	NetworkManager(const std::string_view&);
	~NetworkManager() = default;

private:
	void InitNetwork();
	
	void ProcessMyCharacterPosition(const int inPositionIndex);
	int ProcessOtherCharacterPosition(const int inStartIndex);

private:
	SOCKET socket;
	WSADATA wsa;
	SOCKADDR_IN serverAddr;

	std::string ipAddress;
	char dataBuffer[RECV_BUFFER_SIZE];

	std::array<std::pair<UINT8, UINT8>, GLOBAL_DEFINE::MAX_OTHER_CLIENT + 1> recvCharacterPoistionArr;
};