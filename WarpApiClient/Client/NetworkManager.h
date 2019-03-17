#pragma once

enum class PACKET_TYPE;

class NetworkManager
{
	static constexpr USHORT	SERVER_PORT = 9000;

public:
	std::pair<UINT8, UINT8> SendMoveData(const DIRECTION);

public:
	NetworkManager(const std::string_view&);
	~NetworkManager() = default;

private:
	void InitNetwork();

private:
	std::string ipAddress;
	char dataBuffer[2];

	SOCKET socket;
	WSADATA wsa;
	SOCKADDR_IN serverAddr;
};